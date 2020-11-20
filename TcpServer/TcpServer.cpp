#include "TcpServer.h"
#include "ConnectionChannel.h"

TcpServer::TcpServer(std::shared_ptr <EventLoop> main_event_loop, std::shared_ptr <Acceptor> acceptor, int thread_num) :
    m_main_event_loop(main_event_loop), m_acceptor(acceptor), m_thread_num(thread_num) {
    m_event_loop_thread_pool = std::make_shared<EventLoopThreadPool>(m_main_event_loop, m_thread_num);    
}

TcpServer::~TcpServer() {

}

void TcpServer::Start() {
    m_event_loop_thread_pool->Start();

    auto connection_channel = std::make_shared<ConnectionChannel>(m_acceptor->getListenFd());
    m_main_event_loop->addChannelEvent(connection_channel);
}

EventLoopThreadPool* TcpServer::getEventLoopThreadPool() {
    return m_event_loop_thread_pool.get();
}