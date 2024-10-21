import requests

def upload_to_pastebin(api_key, file_path):
    with open(file_path, 'r') as file:
        file_content = file.read()

    url = "https://pastebin.com/api/api_post.php"
    data = {
        'api_dev_key': api_key,
        'api_option': 'paste',
        'api_paste_code': file_content,
        'api_paste_private': '1'
    }

    response = requests.post(url, data=data)
    if response.status_code == 200:
        print("File uploaded successfully")
        print("Pastebin URL:", response.text)
    else:
        print("Failed to upload file")
        print("Status code:", response.status_code)

if __name__ == "__main__":
    api_key = input("Enter your Pastebin API key: ")
    file_path = input("Enter path of the file to upload: ")
    upload_to_pastebin(api_key, file_path)
