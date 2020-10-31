#include "EventClient.h"

// socket、bind、listen、accept
EventClient::EventClient(std::string addr, int port) {
    // 设置服务端地址
    setServerAddr(addr, port);

    // 初始化socket
    InitSocket();

    // 发起连接
    Connect();
}

// 事件注册
EventClient::EventRegister() {

}

// 事件注销
EventClient::EventUnRegister() {


}

// select
EventClient::EventLoop() {

}

// close
EventClient::~EventClient() {
    DestroySocket();
}

// 设置服务端地址
void EventClient::setServerAddr(char* addr, int port) {
    bzero(&m_server_addr, sizeof(m_server_addr));

    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(port);
    inet_pton(AF_INET, addr, &m_server_addr.sin_addr);

    //m_server_addr_len = sizeof(m_server_addr_len);
}

// 初始化socket
void EventClient::InitSocket() {
    //  AF_INET: IPV4
    //  SOCK_STREAM: TCP
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
}

// 发起连接
void EventClient::Connect() {
    int ret = connect(m_socket_fd, (struct sockaddr *) &m_server_addr, sizeof(m_server_addr));
    if (ret < 0) {
        error(1, errno, "connect failed!");
    }
}

// 关闭连接
void EventClient::DestroySocket() {
    close(m_socket_fd);
}