#include <windows.h>
#include <iostream>

typedef void (*REFLECTIVE_LOADER)(void);

extern "C" __declspec(dllexport) void ReflectiveLoader() {
    MessageBoxA(NULL, "Reflective DLL Injected!", "Success", MB_OK);
}

DWORD WINAPI ReflectiveLoaderEntry(HMODULE hModule) {
    //调用Reflective Loader
    ReflectiveLoader();
    return 0;
}

extern "C" __declspec(dllexport) void ReflectiveLoad(HMODULE hModule) {
    //获取ReflectiveLoader函数地址
    REFLECTIVE_LOADER loader = (REFLECTIVE_LOADER)GetProcAddress(hModule, "ReflectiveLoader");
    if (loader) {
        loader();
    }
}
