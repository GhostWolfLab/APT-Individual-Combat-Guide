#include <windows.h>
#include <stdio.h>

// 枚举窗口的回调函数
BOOL CALLBACK EnumWindowsProc(HWND hWindow, LPARAM parameter) {
    WCHAR windowTitle[1024];
    GetWindowTextW(hWindow, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
    CharUpperW(windowTitle);
    if (wcsstr(windowTitle, L"SYSINTERNALS") != NULL) {
        *(PBOOL)parameter = TRUE;  // 检测到Sysinternals工具
    }
    return TRUE;  // 继续枚举其他窗口
}

int main() {
    BOOL debugged = FALSE;
    EnumWindows(EnumWindowsProc, (LPARAM)(&debugged));
    if (debugged) {
        return 1;  // 检测到调试工具，退出
    }

    MessageBoxW(NULL, L"I am Snowwolf", L"Ghost Wolf Lab", 0);
    return 0;
}
