#ifndef NETWORKACTION_EVENTClient_H
#define NETWORKACTION_EVENTClient_H

#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>    /* inet(3) functions */
#include <unistd.h>
#include <string.h>
#include <errno.h>

class EventClient {
public:
    // socket、connect
    EventClient(std::string addr, int port);

    // 事件注册
    EventRegister();

    // 事件注销
    EventUnRegister();

    // select
    EventLoop();

    // close
    ~EventClient();

private:
    void setServerAddr(char* addr, int port);
    void InitSocket();
    void Connect();
    void DestroySocket();

private:
    int m_socket_fd;
    struct sockaddr_in m_server_addr;
    //socklen_t m_server_addr_len;
};


#endif //NETWORKACTION_EVENTClient_H
