#include <windows.h>
#include <psapi.h>
#include <stdio.h>

bool CheckRunningProcesses() {
    DWORD runningProcessesIDs[1024];
    DWORD runningProcessesCountBytes;

    // 枚举所有进程
    if (!EnumProcesses(runningProcessesIDs, sizeof(runningProcessesIDs), &runningProcessesCountBytes)) {
        return false;  // 无法获取进程信息
    }

    // 计算正在运行的进程数量
    DWORD runningProcessesCount = runningProcessesCountBytes / sizeof(DWORD);

    // 如果进程数量少于50个，可能是沙箱
    return (runningProcessesCount < 50);
}

int main() {
    if (CheckRunningProcesses()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
