#ifndef NETWORKACTION_EVENTClient_H
#define NETWORKACTION_EVENTClient_H

#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>    /* inet(3) functions */
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "EventLoop.h"
#include "Event.h"

class EventClient {
public:
    // socket、connect
    EventClient(std::string addr, int port, std::shared_ptr<EventLoop> event_loop);

    // 事件注册
    void EventRegister(std::shared_ptr<Event> event);

    // 事件注销
    void EventUnRegister(std::shared_ptr<Event> event);

    void EventRun();

    // close
    ~EventClient();

    int getSocketFd() { return m_socket_fd; }

private:
    void setServerAddr(char* addr, int port);
    void InitSocket();
    void Connect();
    void DestroySocket();

private:
    int m_socket_fd;
    struct sockaddr_in m_server_addr;
    std::shared_ptr<EventLoop> m_event_loop;
};


#endif //NETWORKACTION_EVENTClient_H
