# 侦查

##2.1.1

### 1.Whois

whois 域名

https://www.whois.com/whois/

https://centralops.net/co/DomainDossier.aspx

### 2.DNS

dia a 域名

dnsrecon -d 域名 -t axfr

dnsenum 域名

dnsdumpster.com

### 3.CDN

17ce.com

sitereport.netcraft.com

### 4.子域名

searchdns.netcraft.com

crt.sh

#### Aort

```bash
python3 -m venv aort
source aort/bin/activate
pip3 install aort
```

aort -d 域名 --output subdomains.txt

#### Dome

```bash
git clone https://github.com/v4d1/Dome
cd Dome
python3 -m venv Dome
source Dome/bin/activate
pip install -r requirements.txt
```

python3 dome.py -m passive -d 域名

python3 dome.py -d 域名 -m active -w wordlists/wordlist.txt

#### Anubis

```bash
mkdir anubis
cd anubis
python3 -m venv anubis
source anubis/bin/activate
pip3 insyall anubis-netsec
```

anubis -tip 域名 -o 文本文件

anubis -t 域名 --with-nmap -o 文本文件 -i --overwrite-nmap-scan "-F -T5"

#### Subfinder

```bash
go install -v github.com/projectdiscovery/subfinder/v2/cmd/subfinder@latest
```

./subfinder -d 域名

### 5.主机枚举

```bash
hping3 --scan 端口 -S 域名/IP地址
hping3 -S --flood --rand-source -p 端口 IP地址/域名
nmap -A -T4 IP地址/域名
nmap -T4 -p 1-65535 IP地址/域名  //全端口扫描
nmap -T4 IP地址/域名 --spoof-mac 0  //伪造MAC地址
nmap -T4 -f IP地址/域名  //请求包分段
nmap -D IP地址1,IP地址2,...... IP/域名  //伪造IP地址并插入攻击者IP地址
nmap -D RND 随机IP地址数量 IP/域名  //随机IP地址伪造
nmap --spoof-mac 0 --data-length 24 -T4 -f --mtu 16 -D RND -sS -sV -p 1-65535 -n -oA 文本文件 IP/域名
//高隐蔽性扫描

nmap IP地址段 --open -oG scan; cat scan | grep "/open" | cut -d " " -f 2 > active
cat active
//查看地址段存活IP地址

for x in 80 8080 443; do nmap -Pn --host-timeout 201 --max-retries 0 -p $x IP地址段; done
//枚举地址段端口状态
