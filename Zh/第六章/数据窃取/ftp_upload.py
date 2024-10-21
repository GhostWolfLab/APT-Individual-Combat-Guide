import ftplib
import os

def upload_file(ftp_url, username, password, local_file):
    with ftplib.FTP(ftp_url) as ftp:
        ftp.login(user=username, passwd=password)
        with open(local_file, 'rb') as file:
            ftp.storbinary(f'STOR {os.path.basename(local_file)}', file)
        print(f"File '{local_file}' uploaded successfully to '{ftp_url}'")

def main():
    ftp_url = input("Enter FTP URL: ")
    username = input("Enter FTP Username: ")
    password = input("Enter FTP Password: ")
    local_file = input("Enter path of the file to upload: ")

    upload_file(ftp_url, username, password, local_file)

if __name__ == "__main__":
    main()
