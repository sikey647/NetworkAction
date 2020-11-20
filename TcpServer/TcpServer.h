#ifndef NETWORKACTION_TCPSERVER_H
#define NETWORKACTION_TCPSERVER_H

#include <memory>
#include "EventLoop.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"

class TcpServer {
public:
    TcpServer(std::shared_ptr<EventLoop> main_event_loop, 
                std::shared_ptr<Acceptor> acceptor, int thread_num);
    ~TcpServer();
    void Start();
    EventLoopThreadPool* getEventLoopThreadPool();

private:
    static std::shared_ptr<EventLoopThreadPool> m_event_loop_thread_pool;
    std::shared_ptr<EventLoop> m_main_event_loop;
    std::shared_ptr<Acceptor> m_acceptor;
    int m_thread_num;
};


#endif //NETWORKACTION_TCPSERVER_H
