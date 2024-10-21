import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email import encoders
import getpass

def send_email(smtp_server, smtp_port, username, password, to_email, subject, body, file_path):
    msg = MIMEMultipart()
    msg['From'] = username
    msg['To'] = to_email
    msg['Subject'] = subject

    msg.attach(MIMEText(body, 'plain'))

    part = MIMEBase('application', 'octet-stream')
    part.set_payload(open(file_path, 'rb').read())
    encoders.encode_base64(part)
    part.add_header('Content-Disposition', f'attachment; filename={file_path}')
    msg.attach(part)

    server = smtplib.SMTP(smtp_server, smtp_port)
    server.starttls()
    server.login(username, password)
    server.sendmail(username, to_email, msg.as_string())
    server.quit()

if __name__ == "__main__":
    smtp_server = input("Enter SMTP server (网易邮箱服务器: smtp.163.com): ")
    smtp_port = int(input("Enter SMTP port (网易邮箱端口: 25 或 587): "))
    username = input("Enter your email (网易邮箱): ")
    password = getpass.getpass("Enter your email authorization code (网易邮箱授权码): ")
    to_email = input("Enter recipient email: ")
    subject = input("Enter email subject: ")
    body = input("Enter email body: ")
    file_path = input("Enter path of the file to attach: ")

    send_email(smtp_server, smtp_port, username, password, to_email, subject, body, file_path)
