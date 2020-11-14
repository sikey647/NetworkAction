#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ConnectionChannel.h"

ConnectionChannel::ConnectionChannel(int listen_fd) : m_fd(listen_fd) {
    m_event_type = EVENT::READ;
}

ConnectionChannel::~ConnectionChannel() noexcept {

}

int ConnectionChannel::HandleWriteEvent() {

}

int ConnectionChannel::HandleReadEvent() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int conn_fd = accept(m_fd, (struct sockaddr *) &client_addr, &client_len);

    make_nonblocking(conne_fd);

    // get event_loop from thread pool

    // create connection
}

void ConnectionChannel::setNonblocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
}