import smtplib
from email.mime.text import MIMEText
from email.header import Header
import argparse
import getpass

def send_spoofed_email(args, password):
    # 创建 HTML 邮件正文
    email_body_html = args.body.replace('\n', '<br>')
    # 创建邮件消息
    message = MIMEText(email_body_html, 'html', 'utf-8')
    message['From'] = Header(args.name, 'utf-8') # 只设置发件人显示名称
    message['Sender'] = args.from_address # 设置实际的发送者邮箱
    message['To'] = args.to_address
    message['Subject'] = Header(args.subject, 'utf-8')

    try:
        # 连接到SMTP服务器并发送邮件
        server = smtplib.SMTP(args.server, args.port)
        server.starttls()
        server.login(args.username, password)
        server.sendmail(args.from_address, args.to_address, message.as_string())
        print("邮件发送成功！")
    except Exception as e:
        print(f"邮件发送失败：{e}")
    finally:
        server.quit()

if __name__ == "__main__":
    # 创建参数解析器
    parser = argparse.ArgumentParser(description="发送伪造的电子邮件")
    parser.add_argument("-t", "--to", dest="to_address", required=True, help="收件人邮箱地址")
    parser.add_argument("-f", "--from", dest="from_address", required=True, help="伪造的发件人邮箱地址")
    parser.add_argument("-n", "--name", dest="name", required=True, help="发件人显示名称")
    parser.add_argument("-s", "--server", dest="server", required=True, help="SMTP服务器地址")
    parser.add_argument("-p", "--port", dest="port", type=int, required=True, help="SMTP端口号")
    parser.add_argument("-u", "--username", dest="username", required=True, help="SMTP用户名")
    parser.add_argument("-j", "--subject", dest="subject", required=True, help="邮件主题")
    parser.add_argument("-b", "--body", dest="body", required=True, help="邮件正文")
    args = parser.parse_args()

    # 获取密码
    password = getpass.getpass("请输入您的SMTP密码：")

    # 发送邮件
    send_spoofed_email(args, password)
