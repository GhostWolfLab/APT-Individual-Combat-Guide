#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,  DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)  {
    case DLL_PROCESS_ATTACH:
      break;
    case DLL_PROCESS_DETACH:
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) VOID _cdecl GhostWolfLab(void) {
  MessageBox(NULL, "GhostWolfLab", "GhostWolfLab", MB_OK);
}

extern "C" __declspec(dllexport) VOID _cdecl Snowwolf(void) {
  MessageBox(NULL, "Snowwolf", "Snowwolf", MB_OK);
}

extern "C" __declspec(dllexport) VOID _cdecl Superwolf(void) {
  MessageBox(NULL, "Superwolf", "Superwolf", MB_OK);
}
