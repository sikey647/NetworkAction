#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "block_queue.h"

#define BUFFER_SIZE   1024
#define CLIENT_IP_LEN 64
#define THREAD_NUM    4

struct client_info_st {
    int conn_fd;
    char ip[CLIENT_IP_LEN];
    int port;
};

int listen_socket(int port) {
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
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
        perror("bind()");
        exit(-1);
    }

    // 3. 监听
    if (listen(listen_fd, 1024) < 0) {
        perror("listen()");
        exit(-1);
    }

    return listen_fd;
}

void *thread_handler(void *arg) {
    pthread_detach(pthread_self());
    struct client_info_st *client_info_ptr = (struct client_info_st *)arg;

    // 接收数据
    char buff[BUFFER_SIZE];
    bzero(buff, BUFFER_SIZE);
    int recv_ret = recv(client_info_ptr->conn_fd, buff, BUFFER_SIZE, 0);
    if (recv_ret < 0) {
        perror("recv: ");
        close(client_info_ptr->conn_fd);
        exit(-1);
    } else if (recv_ret == 0) {
        fprintf(stderr, "client close socket!\n");
        close(client_info_ptr->conn_fd);
        exit(-1);
    }
    fprintf(stdout, "Client(%s:%d): %s\n", client_info_ptr->ip,
            client_info_ptr->port, buff);

    // 发送数据
    bzero(buff, BUFFER_SIZE);
    strcpy(buff, "Hello, Client!");
    int send_ret = send(client_info_ptr->conn_fd, buff, sizeof(buff), 0);
    if (send_ret < 0) {
        perror("send()");
        close(client_info_ptr->conn_fd);
        exit(-1);
    } else if (send_ret == 0) {
        fprintf(stderr, "client close socket!\n");
        close(client_info_ptr->conn_fd);
        exit(-1);
    }
    fprintf(stdout, "Server[%ld]: %s\n", (unsigned long)client_info_ptr->tid,
            buff);

    // 关闭连接
    close(client_info_ptr->conn_fd);
    free(client_info_ptr);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./tcp_server <ServerPort>\n");
        exit(-1);
    }

    int port = atoi(argv[1]);

    // 创建任务队列

    // 创建线程池
    pthread_t threads[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&threads[i], NULL, thread_handler, (void *)client_info_ptr) < 0) {
            perror("pthread_create()");
            exit(-1);
        }
    }

    // 创建监听 socket
    int listen_fd = listen_socket(port);
    fprintf(stdout, "Server[%ld] running.\n", (unsigned long)pthread_self());

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    while (1) {
        // 接收客户端连接
        int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr,
                             &client_addr_len);
        // 保存客户端地址
        struct client_info_st *client_info_ptr =
            (struct client_info_st *)malloc(sizeof(struct client_info_st));
        if (client_info_ptr == NULL) {
            perror("malloc()");
            continue;
        }
        client_info_ptr->conn_fd = conn_fd;
        bzero(client_info_ptr->ip, CLIENT_IP_LEN);
        inet_ntop(AF_INET, &client_addr, client_info_ptr->ip, CLIENT_IP_LEN);
        client_info_ptr->port = ntohs(client_addr.sin_port);

        // queue.push
    }

    close(listen_fd);

    exit(0);
}

// build:
//  gcc -o tpc_server tpc_server.c -lpthread

// run:
//  ./tpc_server 8080