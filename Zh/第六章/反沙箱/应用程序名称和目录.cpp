#include <windows.h>
#include <stdio.h>

// 检查应用程序名称和路径
bool CheckAppNameAndPath() {
    wchar_t currentProcessPath[MAX_PATH + 1];

    // 获取当前进程的文件路径
    if (GetModuleFileNameW(NULL, currentProcessPath, MAX_PATH + 1) == 0) {
        return false;
    }

    // 转换路径为大写
    CharUpperW(currentProcessPath);

    // 检查文件路径是否包含沙箱特有目录
    if (!wcsstr(currentProcessPath, L"C:\\USERS\\PUBLIC\\")) {
        return false;
    }

    // 检查文件名称是否为可疑名称
    if (!wcsstr(currentProcessPath, L"MALWARE.EXE")) {
        return false;
    }

    return true;  // 符合沙箱检测条件
}

int main() {
    if (CheckAppNameAndPath()) {
        printf("Running inside a sandbox.\n");
    }
    else {
        printf("Not running inside a sandbox.\n");
        MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK);
    }
    return 0;
}
