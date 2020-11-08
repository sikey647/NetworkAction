#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MESSAGE_SIZE 102400

void send_data(int sock_fd) {
    char *query;
    query = malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp;
    cp = query;
    size_t remain = strlen(query);
    while (remain) {
        size_t n_writen = send(sock_fd, cp, remain, 0);
        fprintf(stdout, "send into buffer %ld.\n", n_writen);
        if (n_writen <= 0) {
            fprintf(stderr, "send failed!\n");
            return;
        } else {
            remain -= n_writen;
            cp += n_writen;
        }
    }

    return;
}

int main(int argc, char **argv) {
    int sock_fd;
    struct sockaddr_in serv_addr;

    if (argc != 2) {
        fprintf(stderr, "usage: client <IPaddress>");
        return -1;
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
    int rt = connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (rt < 0) {
        fprintf(stderr, "connect failed!\n");
        return -1;
    }

    send_data(sock_fd);
    return 0;
}