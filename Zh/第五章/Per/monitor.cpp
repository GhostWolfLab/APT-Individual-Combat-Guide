#include "windows.h"
#pragma comment(lib, "winspool")

int main(int argc, char* argv[]) {
  MONITOR_INFO_2 mi;
  mi.pName = "Monitor";
  mi.pEnvironment = "Windows x64";
  mi.pDLLName = "print.dll";
  AddMonitor(NULL, 2, (LPBYTE)&mi);
  return 0;
}
