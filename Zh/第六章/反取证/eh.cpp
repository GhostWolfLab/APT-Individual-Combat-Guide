#include <windows.h>
#include <stdio.h>
#include <tchar.h>

// 未处理异常过滤器函数
LONG WINAPI CustomUnhandledExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo) {
    // 检测调试器
    if (IsDebuggerPresent()) {
        printf("Debugger detected in exception filter!\n");
    }
    else {
        printf("No debugger detected in exception filter.\n");
    }

    // 继续默认处理
    return EXCEPTION_EXECUTE_HANDLER;
}

void trigger_exception() {
    __try {
        // 触发无效内存访问异常
        int* invalid_ptr = NULL;
        *invalid_ptr = 0;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        printf("Exception caught in trigger_exception.\n");
    }
}

int main() {
    // 注册未处理异常过滤器
    SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);

    printf("Starting the program...\n");

    // 触发异常
    trigger_exception();

    printf("Program ended.\n");
    return 0;
}
