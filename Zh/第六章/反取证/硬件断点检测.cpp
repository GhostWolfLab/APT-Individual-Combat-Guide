#include <windows.h>
#include <stdio.h>

// 检查硬件断点的函数
bool CheckHardwareBreakpoints() {
    CONTEXT ctx = { 0 };
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    if (GetThreadContext(GetCurrentThread(), &ctx)) {
        // 检查调试寄存器是否设置
        if (ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3) {
            return true;  // 检测到硬件断点
        }
    }
    return false;  // 没有检测到硬件断点
}

int main() {
    if (CheckHardwareBreakpoints()) {
        printf("Hardware breakpoint detected!\n");
    }
    else {
        printf("No hardware breakpoint detected.\n");
    }
    return 0;
}
