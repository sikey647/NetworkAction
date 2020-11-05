
#include "EventServer.h"

EventServer::EventServer(int port) {
    // 1、设置服务端地址
    setServerAddr(port);

    // 2、初始化 socket
    initSocket();

    // 3、设置 socket属性
    setSocketOpt();

    // 4、绑定
    bindSocket();

    // 5、监听
    listenSocket();

    // 6、信号捕捉
    setSignal();
}

// 设置服务端地址
void EventServer::setServerAddr(int port) {
    bzero(&m_server_addr, sizeof(m_server_addr));
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_server_addr.sin_port = htons(port);
}

// 初始化 socket
void EventServer::initSocket() {
    m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
}

void EventServer::setSocketOpt() {
    int on = 1;
    setsockopt(m_listen_fd, (struct sockaddr *) &m_server_addr, &on, sizeof(on));
}

void EventServer::bindSocket() {
    int ret = bind(m_listen_fd, (struct sockaddr *) &m_server_addr, sizeof(m_server_addr));
    if (ret < 0) {
        error(1, errno, "bind failed!\n");
    }
}

void EventServer::listenSocket() {
    int ret = listen(m_listen_fd, LISTEN_QUEUE);
    if (ret < 0) {
        error(1, errno, "listen failed!\n");
    }
}

void EventServer::setSignal() {
    signal(SIGPIPE, SIG_IGN);
}

