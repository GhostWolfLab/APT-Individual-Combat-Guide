# XXE漏洞

1.检测漏洞

```xml
<!--?xml version="1.0" ?-->
<!DOCTYPE replace [<!ENTITY example "Doe"> ]>
 <userInfo>
  <firstName>John</firstName>
  <lastName>&example;</lastName>
 </userInfo>
```

当 XML 解析器解析外部实体时，结果应在 firstName 中包含John，在 lastName 中包含Doe
