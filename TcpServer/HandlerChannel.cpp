//
// Created by Sikey Liu on 2020/11/16.
//

#include "HandlerChannel.h"

HandlerChannel::HandlerChannel(int conn_fd, Connection *conn, EventLoop *event_loop) :
    Channel(conn_fd),
    m_conn(conn),
    m_event_loop(event_loop) {
    m_event_type = EVENT::READ;
}

HandlerChannel::~HandlerChannel() noexcept {

}

int HandlerChannel::HandleReadEvent(void* data) {
    if (m_conn->readChannelBuffer() > 0) {
        // 应用程序真正读取Buffer里的数据
        m_conn->onChannelMessage();
    } else {
        m_conn->removeChannel();
    }
}

int HandlerChannel::HandleWriteEvent(void* data) {

}

int HandlerChannel::closeHandlerChannel() {

}