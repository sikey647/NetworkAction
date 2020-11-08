#ifndef NETWORKACTION_EVENTSERVER_H
#define NETWORKACTION_EVENTSERVER_H

#include <sys/socket.h>    /* basic socket definitions */
#include <netinet/in.h>    /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>    /* inet(3) functions */
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

const int LISTEN_QUEUE = 1024;

class EventServer {
public:
    EventServer(int port, std::shared_ptr<EventLoop> event_loop);
    virtual ~EventServer();
    void EventRegister(std::shared_ptr<Event> event);
    void EventUnRegister(std::shared_ptr<Event> event);

    void EventRun();

    int getSocketFd() { return m_socket_fd; }

private:
    void setServerAddr(port);
    void initSocket();
    void setSocketOpt();
    void bindSocket();
    void listenSocket();
    void setSignal();

private:
    int m_listen_fd;
    struct sockaddr_in m_server_addr;
};


#endif //NETWORKACTION_EVENTSERVER_H
