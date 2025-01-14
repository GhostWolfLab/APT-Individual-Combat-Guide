import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

# 设置邮件服务器和登录信息
smtp_server = '邮箱服务器地址'
smtp_port = 25
smtp_user = 'SMTP用户名'
smtp_password = '授权码'

# 创建邮件对象
msg = MIMEMultipart('alternative')
msg['From'] = '发件人邮箱地址'
msg['To'] = '收件人邮箱地址'
msg['Subject'] = '主题'

# 读取HTML文件内容
with open('HTML文件', 'r', encoding='utf-8') as file:
    html_content = file.read()

# 添加HTML内容到邮件
msg.attach(MIMEText(html_content, 'html'))

# 发送邮件
with smtplib.SMTP(smtp_server, smtp_port) as server:
    server.starttls()
    server.login(smtp_user, smtp_password)
    server.sendmail(msg['From'], msg['To'], msg.as_string())

print('邮件发送成功')
