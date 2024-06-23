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
