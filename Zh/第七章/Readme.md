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
