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
#include <signal.h>

#define BUFFER_SIZE 1024
#define CLIENT_IP_LEN 64

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
    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
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

void sub_process(struct client_info_st *client_info_ptr) {
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
    fprintf(stdout, "Client(%s:%d): %s\n", client_info_ptr->ip, client_info_ptr->port, buff);

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
    fprintf(stdout, "Server[%d]: %s\n", getpid(), buff);

    // 关闭连接
    close(client_info_ptr->conn_fd);
    free(client_info_ptr);

    return;
}

void sigchld_handler(int sig) {
    while (waitpid(-1, 0, WNOHANG) > 0);
    return;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./tcp_server <ServerPort>\n");
        exit(-1);
    }

    int port = atoi(argv[1]);

    // 创建监听 socket
    int listen_fd = listen_socket(port);
    signal(SIGCHLD, sigchld_handler);
    fprintf(stdout, "Server[%d] running.\n", getpid());

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    while (1) {
        // 接收客户端连接
        int conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_addr_len);
        // 保存客户端地址
        struct client_info_st *client_info_ptr = (struct client_info_st *)malloc(sizeof(struct client_info_st));
        if (client_info_ptr == NULL) {
            perror("malloc()");
            continue;
        }
        client_info_ptr->conn_fd = conn_fd;
        bzero(client_info_ptr->ip, CLIENT_IP_LEN);
        inet_ntop(AF_INET, &client_addr, client_info_ptr->ip, CLIENT_IP_LEN);
        client_info_ptr->port = ntohs(client_addr.sin_port);

        pid_t pid = fork();
        if(pid < 0)
        {
            perror("fork()");
            exit(-1);
        }
        if (pid == 0)	// sub
        {
            close(listen_fd);
            sub_process(client_info_ptr);
            exit(0);
        } else {        // main
            close(conn_fd);
        }
    }

    close(listen_fd);

    exit(0);
}

// build:
//  gcc -o ppc_server ppc_server.c

// run:
//  ./ppc_server 8080