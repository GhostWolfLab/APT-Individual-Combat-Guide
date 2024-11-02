#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <shlwapi.h>
#include <string.h>

#pragma comment(lib, "Shlwapi.lib")

int cmpUnicodeStr(WCHAR substr[], WCHAR mystr[]) {
  _wcslwr_s(substr, MAX_PATH);
  _wcslwr_s(mystr, MAX_PATH);

  int result = 0;
  if (StrStrW(mystr, substr) != NULL) {
    result = 1;
  }
  return result;
}

typedef UINT(CALLBACK* FnMessageBoxA)(
  HWND   hWnd,
  LPCSTR lpText,
  LPCSTR lpCaption,
  UINT   uType
);

//自定义函数
HMODULE MyGetModuleHandle(LPCWSTR lModuleName) {
  PEB* pPeb = (PEB*)__readgsqword(0x60);
  PEB_LDR_DATA* Ldr = pPeb->Ldr;
  LIST_ENTRY* ModuleList = &Ldr->InMemoryOrderModuleList;
  LIST_ENTRY* pStartListEntry = ModuleList->Flink;
  WCHAR mystr[MAX_PATH] = { 0 };
  WCHAR substr[MAX_PATH] = { 0 };
  for (LIST_ENTRY* pListEntry = pStartListEntry; pListEntry != ModuleList; pListEntry = pListEntry->Flink) {
    LDR_DATA_TABLE_ENTRY* pEntry = (LDR_DATA_TABLE_ENTRY*)((BYTE*)pListEntry - sizeof(LIST_ENTRY));
    memset(mystr, 0, MAX_PATH * sizeof(WCHAR));
    memset(substr, 0, MAX_PATH * sizeof(WCHAR));
    wcscpy_s(mystr, MAX_PATH, pEntry->FullDllName.Buffer);
    wcscpy_s(substr, MAX_PATH, lModuleName);
    if (cmpUnicodeStr(substr, mystr)) {
      return (HMODULE)pEntry->DllBase;
    }
  }
  printf("Failed to get a handle to %s\n", lModuleName);
  return NULL;
}

FARPROC MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
  PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
  PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);
  PIMAGE_EXPORT_DIRECTORY exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule +
  ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
  DWORD* addressOfFunctions = (DWORD*)((BYTE*)hModule + exportDirectory->AddressOfFunctions);
  WORD* addressOfNameOrdinals = (WORD*)((BYTE*)hModule + exportDirectory->AddressOfNameOrdinals);
  DWORD* addressOfNames = (DWORD*)((BYTE*)hModule + exportDirectory->AddressOfNames);
  for (DWORD i = 0; i < exportDirectory->NumberOfNames; ++i) {
    if (strcmp(lpProcName, (const char*)hModule + addressOfNames[i]) == 0) {
      return (FARPROC)((BYTE*)hModule + addressOfFunctions[addressOfNameOrdinals[i]]);
    }
  }
  return NULL;
}

//XOR加密后MessageBoxA
unsigned char s_mb[] = { 0x7c, 0x57, 0x40, 0x47, 0x54, 0x6, 0x7, 0x21, 0xb, 0x1d, 0x77 };

//XOR加密后user32.dll
unsigned char s_dll[] = { 0x44, 0x41, 0x56, 0x46, 0x6, 0x53, 0x4c, 0x7, 0x8, 0x9 };

//秘钥
char s_key[] = "12345abcde678910";

void XOR(char * data, size_t data_len, char * key, size_t key_len) {
  int j;
  j = 0;
  for (int i = 0; i < data_len; i++) {
    if (j == key_len - 1) j = 0;
    data[i] = data[i] ^ key[j];
    j++;
  }
}

int main(int argc, char* argv[]) {
  XOR((char *) s_dll, sizeof(s_dll), s_key, sizeof(s_key));
  XOR((char *) s_mb, sizeof(s_mb), s_key, sizeof(s_key));
  wchar_t wtext[20];
  mbstowcs(wtext, s_dll, strlen(s_dll)+1);
  LPWSTR user_dll = wtext;
  HMODULE mod = MyGetModuleHandle(user_dll);
  FnMessageBoxA myMessageBoxA = (FnMessageBoxA)MyGetProcAddress(mod, (LPCSTR)s_mb);
  myMessageBoxA(NULL, "Hi,I am Hide GetProcAddress Function!","Ghost Wolf Lab", MB_OK);
  return 0;
}
