#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

size_t read_n(int fd, void *buffer, size_t size) {
    char *buffer_pointer = buffer;
    int len = size;

    while (len > 0) {
        int res = read(fd, buffer_pointer, len);
        if (res < 0) {
            if (errno == EINTR) {
                continue;   // 考虑非阻塞情况，这里可以再次调用read
            } else {
                return -1;
            }
        } else if (res == 0) {
            break;
        } else { // res > 0
            len -= res;
            buffer_pointer += res;
        }
    }

    return size - len;  // 返回实际读取的字节数
}

void read_data(int sock) {
    char buf[1024];

    int time = 0;
    while (1) {
        fprintf(stdout, "block in read\n");
        ssize_t n = read_n(sock, buf, 1024);
        if (n < 1) {
            fprintf(stderr, "read_n error: %d\n", (int)n);
            return;
        } else if (n == 0) {
            close(sock);
            return;
        } else {
            time++;
            fprintf(stdout, "1K read for %d\n", time);
            usleep(1000);
        }
    }
}

int main(int argc, char **argv) {
    int listen_fd, conn_fd;
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(12345);

    bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(listen_fd, 1024);

    cli_len = sizeof(cli_addr);
    conn_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &cli_len);
    read_data(conn_fd);
    close(conn_fd);
}