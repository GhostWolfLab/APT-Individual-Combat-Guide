#include <windows.h>
#include <stdio.h>

typedef NTSTATUS(WINAPI* PNtDelayExecution)(IN BOOLEAN Alertable, IN PLARGE_INTEGER DelayInterval);

bool CheckSandboxByDelayingExecution() {
    ULONGLONG uptimeBeforeSleep = GetTickCount64();

    // 动态获取 NtDelayExecution 函数地址
    PNtDelayExecution pNtDelayExecution = (PNtDelayExecution)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDelayExecution");
    if (!pNtDelayExecution) {
        return false;
    }

    // 设置延迟时间（100秒）
    LARGE_INTEGER delay;
    delay.QuadPart = -10000 * 100000;  // 100 seconds in 100-nanosecond intervals

    // 调用 NtDelayExecution 进行休眠
    pNtDelayExecution(FALSE, &delay);

    ULONGLONG uptimeAfterSleep = GetTickCount64();

    // 检查休眠前后的系统正常运行时间差
    if ((uptimeAfterSleep - uptimeBeforeSleep) < 100) {  // 沙箱可能快进了模拟时间
        return true;  // 可能在沙箱环境中
    }

    return false;  // 不是沙箱环境
}

int main() {
    if (CheckSandboxByDelayingExecution()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
