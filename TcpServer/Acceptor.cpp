#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Acceptor.h"

Acceptor::Acceptor(int port) : m_port(port) {
    m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    setNonblocking();

    int on = 1;
    setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(m_port);

    if (bind(m_listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind()");
        exit(-1);
    }

    if (listen(m_listen_fd, LISTENQ) < 0) {
        perror("listen()");
        exit(-1);
    }
}

int Acceptor::getListenFd() {
    return m_listen_fd;
}

Acceptor::~Acceptor() {
    close(m_listen_fd);
}

void Acceptor::setNonblocking() {
    fcntl(m_listen_fd, F_SETFL, O_NONBLOCK);
}