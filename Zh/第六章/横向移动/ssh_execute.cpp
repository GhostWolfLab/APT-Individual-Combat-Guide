#include <iostream>
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    const char *hostname = "192.168.0.165";  //远程主机IP地址
    const char *username = "root";  //远程主机SSH的用户名
    const char *password = "123qwe";  //远程主机SSH的密码
    const char *command = "dir";  //远程主机执行的命令

    //初始化libssh2
    libssh2_init(0);

    //创建socket连接到目标主机
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "无法创建socket。" << std::endl;
        return 1;
    }

    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(22);  //SSH端口
    inet_pton(AF_INET, hostname, &target.sin_addr);

    if (connect(sock, (struct sockaddr*)&target, sizeof(target)) == -1) {
        std::cerr << "无法连接到目标主机。" << std::endl;
        close(sock);
        return 1;
    }

    //创建会话
    LIBSSH2_SESSION *session = libssh2_session_init();
    if (!session) {
        std::cerr << "无法初始化SSH会话。" << std::endl;
        close(sock);
        return 1;
    }

    //启动会话
    libssh2_session_handshake(session, sock);

    //进行身份验证
    libssh2_userauth_password(session, username, password);

    //执行命令
    LIBSSH2_CHANNEL *channel = libssh2_channel_open_session(session);
    libssh2_channel_exec(channel, command);

    //读取命令输出
    char buffer[256];
    int bytes_read;
    while ((bytes_read = libssh2_channel_read(channel, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        std::cout << buffer;
    }

    //关闭通道和会话
    libssh2_channel_close(channel);
    libssh2_channel_free(channel);
    libssh2_session_disconnect(session, "Normal Shutdown");
    libssh2_session_free(session);
    libssh2_exit();

    close(sock);
    return 0;
}
