#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

size_t read_n(int fd, viod *buffer, size_t size) {
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