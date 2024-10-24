#include <windows.h>
#include <stdio.h>

// 定义TLS回调函数
void NTAPI TlsCallback(PVOID DllHandle, DWORD dwReason, PVOID) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        // 在进程启动时检测调试器
        if (IsDebuggerPresent()) {
            exit(0);
        }
    }
}

// 使用链接器指令注册TLS回调函数
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:tls_callback_function")

// 将TLS回调函数绑定到特定段
#pragma const_seg(".CRT$XLA")
EXTERN_C const PIMAGE_TLS_CALLBACK tls_callback_function = TlsCallback;
#pragma const_seg()

int main() {
    MessageBoxW(NULL, L"I am Snowwolf", L"Ghost Wolf Lab", 0);
    return 0;
}
