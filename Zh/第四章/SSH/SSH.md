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
