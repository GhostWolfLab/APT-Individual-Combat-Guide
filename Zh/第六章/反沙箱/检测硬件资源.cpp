#include <windows.h>
#include <stdio.h>

bool IsSandbox() {
    // 检查CPU核心数
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    if (systemInfo.dwNumberOfProcessors < 2) {
        return true;  // 可能是沙箱
    }

    // 检查RAM大小
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    if (memoryStatus.ullTotalPhys / 1024 / 1024 < 2048) {
        return true;  // 可能是沙箱
    }

    // 检查硬盘容量
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
    if (GetDiskFreeSpaceEx(L"C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        if (totalNumberOfBytes.QuadPart / 1024 / 1024 / 1024 < 100) {
            return true;  // 可能是沙箱
        }
    }

    return false;
}

int main() {
    if (IsSandbox()) {
        printf("Running inside a sandbox.\n");
    }
    else {
        printf("Not running inside a sandbox.\n");
        MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK | MB_ICONWARNING);
    }
    return 0;
}
