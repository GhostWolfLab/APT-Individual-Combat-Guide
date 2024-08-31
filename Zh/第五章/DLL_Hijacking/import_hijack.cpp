#include <windows.h>
#include <cstdio>

typedef VOID (__cdecl *GhostProc)();
typedef VOID (__cdecl *SnowProc)();
typedef VOID (__cdecl *SuperProc)();

int main(void) {

  //具有导出函数的Main
  HINSTANCE ImportDLL;
  GhostProc GhostFunc;
  SnowProc SnowFunc;
  SuperProc SuperFunc;
  //空闲内存
  BOOL freeRes;

  //加载import.dll
  ImportDLL = LoadLibrary(TEXT("import.dll"));

  if (ImportDLL != NULL) {
    GhostFunc = (GhostProc) GetProcAddress(ImportDLL, "GhostWolfLab");
    SnowFunc = (SnowProc) GetProcAddress(ImportDLL, "Snowwolf");
    SuperFunc = (SuperProc) GetProcAddress(ImportDLL, "Superwolf");
    if (GhostFunc != NULL) {
      (GhostFunc) ();
    }
    if (SnowFunc != NULL) {
      (SnowFunc) ();
    }
    if (SuperFunc != NULL) {
      (SuperFunc) ();
    }
    freeRes = FreeLibrary(ImportDLL);
  }

  return 0;
}
