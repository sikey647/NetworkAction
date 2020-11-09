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

#define BUFFER_SIZE   1024
#define CLIENT_IP_LEN 64

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./tcp_server <ServerPort>\n");
        exit(-1);
    }

    int port = atoi(argv[1]);

    // 0. 初始化服务端地址（Port）
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // 1. 创建监听socket
    int listen_fd;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 绑定本地地址
    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind()");
        exit(-1);
    }

    // 3. 监听
    if (listen(listen_fd, 1024) < 0) {
        perror("listen()");
        exit(-1);
    }

    // 循环处理客户端请求
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    while (1) {
        // 4. 接收客户端的连接
        int conn_fd;
        conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        // 保存客户端地址
        char client_ip[CLIENT_IP_LEN];
        bzero(client_ip, CLIENT_IP_LEN);
        inet_ntop(AF_INET, &client_addr, client_ip, CLIENT_IP_LEN);
        int client_port = ntohs(client_addr.sin_port);


        // 5. 接收数据
        char buff[BUFFER_SIZE];
        bzero(buff, BUFFER_SIZE);
        int recv_ret = recv(conn_fd, buff, BUFFER_SIZE, 0);
        if (recv_ret < 0) {
            perror("recv()");
            close(conn_fd);
            exit(-1);
        } else if (recv_ret == 0) {
            fprintf(stderr, "client close socket!\n");
            close(conn_fd);
            exit(-1);
        }
        fprintf(stdout, "Client(%s:%d): %s\n", client_ip, client_port, buff);

        // 6. 发送数据
        bzero(buff, BUFFER_SIZE);
        strcpy(buff, "Hello, Client!");
        int send_ret = send(conn_fd, buff, sizeof(buff), 0);
        if (send_ret < 0) {
            perror("send()");
            close(conn_fd);
            exit(-1);
        } else if (send_ret == 0) {
            fprintf(stderr, "client close socket!\n");
            close(conn_fd);
            exit(-1);
        }
        fprintf(stdout, "Server: %s\n", buff);

        // 7. 关闭连接
        close(conn_fd);
    }

    close(listen_fd);

    exit(0);
}

// build:
//  gcc -o tcp_server tcp_server.c

// run
//  ./tcp_server 8080

