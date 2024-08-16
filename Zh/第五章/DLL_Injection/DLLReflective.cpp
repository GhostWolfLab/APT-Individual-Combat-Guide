#include <windows.h>
#include <iostream>

typedef void (*SHELLCODE)(void);

extern "C" __declspec(dllexport) void ReflectiveLoader() {
    //从DLL加载Shellcode
    SHELLCODE shellcode = (SHELLCODE)0x41414141; //替换为实际的Shellcode地址
    shellcode();
}

extern "C" __declspec(dllexport) void ReflectiveLoad(HMODULE hModule) {
    //获取ReflectiveLoader函数地址
    void (*loader)() = (void (*)())GetProcAddress(hModule, "ReflectiveLoader");
    if (loader) {
        loader();
    }
}
