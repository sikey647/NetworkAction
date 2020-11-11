#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define CLIENT_IP_LEN 64
#define MAX_EVENTS 128

void set_nonblock(int fd) { fcntl(fd, F_SETFL, O_NONBLOCK); }

int make_nonblock_listen_socket(int port) {
    // 0. 初始化服务端地址（Port）
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // 1. 创建监听socket
    int listen_fd;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    //  设置非阻塞
    set_nonblock(listen_fd);

    //  设置地址重用
    int on = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

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

    signal(SIGPIPE, SIG_IGN);

    return listen_fd;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./tcp_server <ServerPort>\n");
        exit(-1);
    }

    int port = atoi(argv[1]);

    // 创建监听 socket
    int listen_fd = make_nonblock_listen_socket(port);
    fprintf(stdout, "Server running.\n");

    // 创建epoll实例
    struct epoll_event event;
    struct epoll_event *events;

    int efd = epoll_create1(0);
    if (efd < 0) {
        perror("epoll_create()1");
        exit(-1);
    }

    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
        perror("epoll_ctl");
        exit(-1);
    }

    events = calloc(MAX_EVENTS, sizeof(event));

    while (1) {
        int n = epoll_wait(efd, events, MAX_EVENTS, 1);
        fprintf(stdout, "epoll_wait done.\n");

        int i;
        for (i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN))) {
                fprintf(stderr, "epoll error\n");
                close(events[i].data.fd);
                continue;
            } else if (listen_fd == events[i].data.fd) {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr,
                                     &client_addr_len);
                if (conn_fd < 0) {
                    perror("accept()");
                    continue;
                }
                set_nonblock(conn_fd);
                event.data.fd = conn_fd;
                event.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(efd, EPOLL_CTL_ADD, conn_fd, &event) == -1) {
                    perror("epoll_ctl()");
                }
                continue;
            } else {
                int conn_fd = events[i].data.fd;
                char buff[BUFFER_SIZE];
                bzero(buff, BUFFER_SIZE);
                int read_ret = read(conn_fd, buff, sizeof(buff));
                if (read_ret < 0) {
                    if (errno != EAGAIN) {
                        perror("read()");
                        close(conn_fd);
                    }
                    continue;
                } else if (read_ret == 0) {
                    close(conn_fd);
                    continue;
                } else {
                    fprintf(stdout, "Client: %s\n", buff);
                    fprint
                    bzero(buff, BUFFER_SIZE);
                    strcpy(buff, "Hello, Client!");
                    int send_ret = send(conn_fd, buff, sizeof(buff), 0);
                    if (send_ret < 0) {
                        if (errno != EAGAIN) {
                            perror("send()");
                            close(conn_fd);
                        }
                        continue;
                    } else if (send_ret == 0) {
                        fprintf(stderr, "client close socket!\n");
                        close(conn_fd);
                        return;
                    }
                    fprintf(stdout, "Server: %s\n", buff);
                }
            }
        }
    }

    free(events);
    close(listen_fd);

    exit(0);
}