# SSH服务

SSH服务器
+ [OpenSSH](http://www.openssh.org/)
+ [DropBear](https://matt.ucc.asn.au/dropbear/dropbear.html)
+ [Putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/)
+ [CopSSH](https://www.itefix.net/copssh)

Banner
```bash
nc -vn <IP> 22
```

用户名枚举
```bash
msf> use scanner/ssh/ssh_enumusers
```

暴力破解

[默认字典](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%9B%9B%E7%AB%A0/SSH/ssh-betterdefaultpasslist.txt)
[20个最常用的](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%9B%9B%E7%AB%A0/SSH/top-20-common-SSH-passwords.txt)
