#ifndef NETWORKACTION_TCPSERVER_H
#define NETWORKACTION_TCPSERVER_H

#include <memory>
#include "EventLoop.h"
#include "Acceptor.h"

class TcpServer {
public:
    TcpServer(std::shared_ptr<EventLoop> event_loop, std::shared_ptr<Acceptor> acceptor);
    ~TcpServer();
    void Start();

private:
    std::shared_ptr<EventLoop> m_event_loop;
    std::shared_ptr<Acceptor> m_acceptor;
};


#endif //NETWORKACTION_TCPSERVER_H
