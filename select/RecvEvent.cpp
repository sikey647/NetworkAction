//
// Created by Sikey Liu on 2020/11/2.
//
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "RecvEvent.h"

void RecvEvent::work() {
    int n = read(m_event_fd, m_recv_line, MAX_LINE);
    if (n < 0) {
        error(1, errno, "read error");
    } else if (n == 0) {
        error(1, errno, "server terminated \n");
    }
    m_recv_line[n] = 0;
    fputs(m_recv_line, stdout);
    fputs("\n", stdout);
}