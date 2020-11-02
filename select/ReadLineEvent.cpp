#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "ReadLineEvent.h"

void ReadLineEvent::work() {
    if (fgets(m_send_line, MAX_LINE, stdin)) {
        int i = strlen(m_send_line);
        if (m_send_line[i - 1] == '\n') {
            m_send_line[i - 1] = 0;
        }
        printf("now sending %s\n", m_send_line);

        ssize_t rt = write(m_send_socket_fd, m_send_line, strlen(m_send_line));
        if (rt < 0) {
            errno(1, errno, "write failed");
        }
        printf("send byte: %zu\n", rt);
    }
}