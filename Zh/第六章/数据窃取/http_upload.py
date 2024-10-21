import requests

def send_data(file_path, server_url):
    with open(file_path, 'rb') as f:
        files = {'file': f}
        response = requests.post(server_url, files=files)
        print(f"Server response: {response.text}")

def main():
    file_path = input("Enter path of the file to upload: ")
    server_url = input("Enter the server URL to upload to: ")
    send_data(file_path, server_url)

if __name__ == "__main__":
    main()
