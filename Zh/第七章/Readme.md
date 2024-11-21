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
