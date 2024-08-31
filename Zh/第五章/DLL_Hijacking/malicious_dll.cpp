#include <windows.h>

extern "C" __declspec(dllexport) void HookedFunction() {
    MessageBox(NULL, "Function Hijacked!", "Alert", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // 在这里可以进行IAT劫持
HijackIAT(hModule, "GetFileVersionInfoW", HookedFunction);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
