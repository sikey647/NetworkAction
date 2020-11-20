#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "TcpServer.h"
#include "ConnectionChannel.h"
#include "HandlerChannel.h"

ConnectionChannel::ConnectionChannel(int listen_fd, EventLoop *event_loop) : Channel(listen_fd, EVENT::READ), m_event_loop(event_loop) {}

ConnectionChannel::~ConnectionChannel() { }

int ConnectionChannel::HandleWriteEvent(void* data) {

}

int ConnectionChannel::HandleReadEvent(void* data) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int conn_fd = accept(m_fd, (struct sockaddr *) &client_addr, &client_len);

    setNonblocking(conn_fd);

    // get event_loop from thread pool
    EventLoop *event_loop = m_event_loop_thread_pool->getEventLoop();

    // create connection
    auto handler_channel = std::make_shared<HandlerChannel>(conn_fd, event_loop);
    event_loop->addChannelEvent(handler_channel);
}

void ConnectionChannel::setNonblocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
}