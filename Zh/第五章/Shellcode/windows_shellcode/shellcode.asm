section .text
global _start

_start:
    ; 清除RAX寄存器
    xor rax, rax

    ; 将字符串"cmd.exe"压入堆栈
    push rax
    mov rbx, 0x636d642e6578652e
    push rbx

    ; 设置寄存器
    mov rdi, rsp          ; RDI指向字符串"cmd.exe"
    xor rsi, rsi          ; RSI为空
    xor rdx, rdx          ; RDX为空

    ; 调用execve
    mov rax, 0x3b         ; 系统调用号：execve
    syscall
