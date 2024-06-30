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
enum4linux -a IP地址  //获取RID 范围、用户名、工作组、Nbtstat、会话、SID、系统等信息
enum4linux -a -l -s IP地址  //枚举共享目录
smbmap -H IP地址  //枚举整域共享和权限
smbclient -L //<目标IP>  //列出目标系统上的SMB共享

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
