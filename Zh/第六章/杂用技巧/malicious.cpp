#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        MessageBox(NULL, "DLL Injected!", "Alert", MB_OK | MB_ICONINFORMATION);
        WinExec("calc.exe", SW_SHOW);
    }
    return TRUE;
}
