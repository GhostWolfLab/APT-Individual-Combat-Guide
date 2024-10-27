#include <windows.h>
#include <stdio.h>

bool CheckSandboxFiles() {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(L"C:\\SandboxIndicatorFile.txt", &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        FindClose(hFind);
        return true;  // 检测到沙箱文件
    }
    return false;  // 未检测到沙箱文件
}

int main() {
    if (CheckSandboxFiles()) {
        printf("Running inside a sandbox.\n");
    }
    else {
        printf("Not running inside a sandbox.\n");
        MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK);
    }
    return 0;
}
