#include <windows.h>

__declspec(dllexport) void ExecuteModuleA() {
    MessageBox(NULL, "Module A executed", "Alert", MB_OK | MB_ICONINFORMATION);
}
