#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: ./tcp_client <Sever IP> <Port>\n");
        exit(-1);
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    // 0. 初始化服务端地址（IP+Port）
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    // 1. 创建socket
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 建立连接
    int conn_ret = connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (conn_ret < 0) {
        perror("connect()");
        exit(-1);
    }
    fprintf(stdout, "connect Server(%s:%d) success.\n", ip, port);

    // 3. 发送数据
    char buff[BUFFER_SIZE];
    bzero(buff, BUFFER_SIZE);
    strcpy(buff, "Hello, Server!");
    int send_ret = send(sock_fd, buff, sizeof(buff), 0);
    if (send_ret < 0) {
        perror("send()");
        close(sock_fd);
        exit(-1);
    } else if (send_ret == 0) {
        fprintf(stderr, "server close socket!\n");
        close(sock_fd);
        exit(-1);
    }
    fprintf(stdout, "Client: %s\n", buff);

    // 4. 接收数据
    bzero(buff, BUFFER_SIZE);
    int recv_ret = recv(sock_fd, buff, BUFFER_SIZE, 0);
    if (recv_ret < 0) {
        perror("recv()");
        close(sock_fd);
        exit(-1);
    } else if (recv_ret == 0) {
        fprintf(stderr, "server close socket!\n");
        close(sock_fd);
        exit(-1);
    }
    fprintf(stdout, "Server: %s\n", buff);

    // 5、关闭连接
    close(sock_fd);

    exit(0);
}

// build:
//  gcc -o tcp_client tcp_client.c

// run
//  ./tcp_client 127.0.0.1 8080