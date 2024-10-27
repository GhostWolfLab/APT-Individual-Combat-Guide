#include <windows.h>
#include <stdio.h>

// 检查系统正常运行时间
bool CheckUptime() {
    ULONGLONG uptime = GetTickCount64() / 1000;  // 将毫秒转换为秒

    // 如果系统运行时间少于20分钟，可能是沙箱
    return (uptime < 1200);
}

int main() {
    if (CheckUptime()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
