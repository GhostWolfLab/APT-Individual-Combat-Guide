#include <windows.h>
#include <stdio.h>

// 回调函数，用于检查每个显示器的分辨率
BOOL CALLBACK MyCallback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lpRect, LPARAM data) {
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfoW(hMonitor, &monitorInfo);

    int xResolution = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
    int yResolution = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

    if ((xResolution != 1920 && xResolution != 2560 && xResolution != 1440) ||
        (yResolution != 1080 && yResolution != 1200 && yResolution != 1600 && yResolution != 900)) {
        *((BOOL*)data) = TRUE;
    }

    return TRUE;
}

int main() {
    // 检查主显示器的分辨率
    int xResolution = GetSystemMetrics(SM_CXSCREEN);
    int yResolution = GetSystemMetrics(SM_CYSCREEN);

    if (xResolution < 1000 && yResolution < 1000) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    // 枚举所有显示器并检查分辨率
    BOOL sandbox = FALSE;
    EnumDisplayMonitors(NULL, NULL, MyCallback, (LPARAM)(&sandbox));

    if (sandbox) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);

    return 0;
}
