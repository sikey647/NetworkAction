#include "TcpServer.h"
#include "ConnectionChannel.h"

TcpServer::TcpServer(std::shared_ptr <EventLoop> event_loop, std::shared_ptr <Acceptor> acceptor) :
    m_event_loop(event_loop), m_acceptor(acceptor) {

}

TcpServer::~TcpServer() {

}

void TcpServer::Start() {
    // create thread pool

    // 接收客户端连接
    // 需要传入 this
    m_event_loop->addChannelEvent(std::make_shared<ConnectionChannel>(m_acceptor->getListenFd()));
}