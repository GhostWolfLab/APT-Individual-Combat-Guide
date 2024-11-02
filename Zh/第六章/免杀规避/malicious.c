#include <windows.h>
#include <stdio.h>

void MaliciousCode() {
    MessageBoxA(NULL, "Malicious code executed!", "Alert", MB_OK | MB_ICONWARNING);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        MaliciousCode();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
