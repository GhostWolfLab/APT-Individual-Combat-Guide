## 加载器

### 下载远程恶意文件

[wolf.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E5%8A%A0%E8%BD%BD%E5%99%A8/wolf.cpp)

```bash
x86_64-w64-mingw32-g++ -shared -o wolf.dll wolf.cpp -fpermissive
```

[download.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E5%8A%A0%E8%BD%BD%E5%99%A8/download.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 download.cpp -o download.exe -mconsole -lwininet -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```bash
.\download.exe 进程PID
```

### Shellcode加载

1. EnumDesktopsA函数

```bash
msfvenom -p windows/x64/messagebox TEXT="Hello, World" -f c
```

[enumdesktopa.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E5%8A%A0%E8%BD%BD%E5%99%A8/enumdesktopa.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 enumdesktopa.cpp -o enumdesktopa.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
````

2. EnumChildWindows函数

[ecw.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E5%8A%A0%E8%BD%BD%E5%99%A8/ecw.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 ecw.cpp -o ecw.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

3. Listplanting函数

[llcalc.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E5%8A%A0%E8%BD%BD%E5%99%A8/llcalc.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 llcalc.cpp -o llcalc.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

4. EnumerateLoadedModules函数

[elm.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E5%8A%A0%E8%BD%BD%E5%99%A8/elm.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 elm.cpp -o elm.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive -ldbghelp
```

## 系统调用

[sycall.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E7%B3%BB%E7%BB%9F%E8%B0%83%E7%94%A8/sycall.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 sycall.cpp -o sycall.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```
