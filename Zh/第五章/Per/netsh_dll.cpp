#include <windows.h>
#pragma comment (lib, "user32.lib")

extern "C" __declspec(dllexport) DWORD InitHelperDll(DWORD dwNetshVersion, PVOID pReserved) {
  MessageBox(NULL, "I am SnowWolf!", "GhostWolfLab", MB_OK);
  return 0;
}
