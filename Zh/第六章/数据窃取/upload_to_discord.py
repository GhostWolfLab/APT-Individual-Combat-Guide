import requests
import subprocess
import argparse
import os

WEBHOOK_URL = '[Discord Webhook URL]'  # 替换为你的Discord Webhook URL

def send_to_discord(content):
    data = {
        "content": content
    }
    response = requests.post(WEBHOOK_URL, json=data)
    if response.status_code == 204:
        print("Message posted successfully.")
    else:
        print(f"Failed to post message: {response.status_code}")

def main():
    parser = argparse.ArgumentParser(description='Upload command result or file content to Discord.')
    parser.add_argument('-c', '--command', help='Command to execute')
    parser.add_argument('-f', '--file', help='File whose content to upload')
    args = parser.parse_args()

    if args.command:
        result = subprocess.run(args.command, shell=True, capture_output=True, text=True)
        content = result.stdout or result.stderr
    elif args.file:
        try:
            with open(args.file, 'r') as file:
                content = file.read()
        except Exception as e:
            content = f"Error reading file: {e}"
    else:
        content = "No command or file specified."

    send_to_discord(content)

if __name__ == "__main__":
    main()
