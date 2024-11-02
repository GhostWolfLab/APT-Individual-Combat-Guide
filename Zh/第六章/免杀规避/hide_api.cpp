#include <stdio.h>
#include "windows.h"

typedef UINT(CALLBACK* fnMessageBoxA)(
  HWND   hWnd,
  LPCSTR lpText,
  LPCSTR lpCaption,
  UINT   uType
);

//XOR加密后的user32.dll
unsigned char s_dll[] = { 0x44, 0x41, 0x56, 0x46, 0x6, 0x53, 0x4c, 0x7, 0x8, 0x9 };
//XOR加密后的MessageBoxA
unsigned char s_mb[] = { 0x7c, 0x57, 0x40, 0x47, 0x54, 0x6, 0x7, 0x21, 0xb, 0x1d, 0x77 };
char s_key[] = "12345abcde678910";  //秘钥

//XOR解密
void XOR(char * data, size_t data_len, char * key, size_t key_len) {
  int j;
  j = 0;
  for (int i = 0; i < data_len; i++) {
    if (j == key_len - 1) j = 0;
    data[i] = data[i] ^ key[j];
    j++;
  }
}

//二分查找
DWORD findFuncB(PDWORD npt, DWORD size, PBYTE base, LPCSTR proc) {
  INT   cmp;
  DWORD max;
  DWORD mid;
  DWORD min;

  min = 0;
  max = size - 1;

  while (min <= max) {
    mid = (min + max) >> 1;
    cmp = strcmp((LPCSTR)(npt[mid] + base), proc);

    if (cmp < 0) {
      min = mid + 1;
    } else if (cmp > 0) {
      max = mid - 1;
    } else {
      return mid;
    }
  }
  return -1;
}

//获取导出目录表
PIMAGE_EXPORT_DIRECTORY getEDT(HMODULE module) {
  PBYTE           base;
  PIMAGE_FILE_HEADER    img_file_header;
  PIMAGE_EXPORT_DIRECTORY edt;
  DWORD           rva;
  PIMAGE_DOS_HEADER     img_dos_header;
  PIMAGE_OPTIONAL_HEADER  img_opt_header;
  PDWORD          sig;
  base = (PBYTE)module;
  img_dos_header = (PIMAGE_DOS_HEADER)module;

  //获取PE签名并验证
  sig = (DWORD*)(base + img_dos_header->e_lfanew);
  if (IMAGE_NT_SIGNATURE != *sig) {
    return NULL;
  }

  //获取COFF文件头
  img_file_header = (PIMAGE_FILE_HEADER)(sig + 1);

  //获取可选标头
  img_opt_header = (PIMAGE_OPTIONAL_HEADER)(img_file_header + 1);
  if (IMAGE_DIRECTORY_ENTRY_EXPORT >= img_opt_header->NumberOfRvaAndSizes) {
    return NULL;
  }
  rva = img_opt_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
  edt = (PIMAGE_EXPORT_DIRECTORY)(base + rva);

  return edt;
}

//获取函数序数
DWORD getFuncOrd(HMODULE module, LPCSTR proc) {
  PBYTE           base;
  PIMAGE_EXPORT_DIRECTORY edt;
  PWORD           eot;
  DWORD           i;
  PDWORD          npt;

  base = (PBYTE)module;
  edt = getEDT(module);

//获取名称指针表并在其中搜索命名过程
  npt = (DWORD*)(base + edt->AddressOfNames);
  i = findFuncB(npt, edt->NumberOfNames, base, proc);
  if (-1 == i) {
    return -1;
  }

  eot = (WORD*)(base + edt->AddressOfNameOrdinals);  //获取导出序数表
  return eot[i] + edt->Base;
}

int main(int argc, char* argv[]) {
  XOR((char *) s_dll, sizeof(s_dll), s_key, sizeof(s_key));
  XOR((char *) s_mb, sizeof(s_mb), s_key, sizeof(s_key));

  if (NULL == LoadLibrary((LPCSTR) s_dll)) {
    printf("Failed to load library %s\n", s_dll);
    return -2;
  }

  HMODULE module = GetModuleHandle((LPCSTR) s_dll);
  if (NULL == module) {
    printf("Failed to get a handle to %s\n", s_dll);
    return -2;
  }

  DWORD ord = getFuncOrd(module, (LPCSTR) s_mb);
  if (-1 == ord) {
    printf("Failed to find ordinal %s\n", s_mb);
    return -2;
  }

  fnMessageBoxA myMessageBoxA = (fnMessageBoxA)GetProcAddress(module, MAKEINTRESOURCE(ord));
  myMessageBoxA(NULL, "Hi,I am Hide Windows API Function","Ghost Wolf Lab", MB_OK);
  return 0;
}
