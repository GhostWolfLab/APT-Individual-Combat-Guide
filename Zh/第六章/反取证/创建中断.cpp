#include <windows.h>
#include <stdio.h>

#pragma auto_inline(off)

// 触发软件断点
void TriggerBreakpoint() {
    __debugbreak();  // 使用 __debugbreak 函数触发断点
}

int main() {
    printf("Starting program...\n");

    __try {
        TriggerBreakpoint();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        printf("Exception caught! Detected debugger.\n");
    }

    printf("Program ended.\n");
    return 0;
}
