#include <windows.h>
#include <shlobj.h>
#include <strsafe.h>
#include <stdio.h>

// 检查最近访问的文档文件夹中的项目数量
bool CheckRecentDocuments() {
    PWSTR recentFolder = NULL;

    // 获取最近访问的文档文件夹路径
    if (SHGetKnownFolderPath(FOLDERID_Recent, 0, NULL, &recentFolder) != S_OK) {
        return false;
    }

    wchar_t recentFolderFiles[MAX_PATH + 1] = L"";
    StringCchCopyW(recentFolderFiles, MAX_PATH, recentFolder);
    StringCchCatW(recentFolderFiles, MAX_PATH, L"\\*");

    int numberOfRecentFiles = 0;
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(recentFolderFiles, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            numberOfRecentFiles++;
        } while (FindNextFileW(hFind, &findFileData));
        FindClose(hFind);
    }

    CoTaskMemFree(recentFolder);

    if (numberOfRecentFiles >= 2) {
        numberOfRecentFiles -= 2;  // 排除 '.' 和 '..'
    }

    // 如果最近访问的文件少于20个，可能是沙箱
    return (numberOfRecentFiles < 20);
}

int main() {
    if (CheckRecentDocuments()) {
        printf("Running inside a sandbox.\n");
        return 0;
    }

    printf("Not running inside a sandbox.\n");

    // 这里插入恶意代码，比如显示一个弹窗消息
    MessageBoxA(NULL, "You have been infected by a malware!", "Malware Alert", MB_OK);

    return 0;
}
