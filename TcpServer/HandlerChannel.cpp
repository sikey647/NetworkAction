//
// Created by Sikey Liu on 2020/11/16.
//

#include "HandlerChannel.h"

HandlerChannel::HandlerChannel(int conn_fd, Connection *conn, EventLoop *event_loop) :
    Channel(conn_fd, EVENT::READ),
    m_conn(conn),
    m_event_loop(event_loop) {
}

HandlerChannel::~HandlerChannel() {

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
    m_conn->writeChannelBuffer();
}