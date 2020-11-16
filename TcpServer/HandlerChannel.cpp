//
// Created by Sikey Liu on 2020/11/16.
//

#include "HandlerChannel.h"

HandlerChannel::HandlerChannel(int conn_fd, Connection *conn) :
    Channel(conn_fd),
    m_conn(conn) {
    m_event_type = EVENT::READ;
}

HandlerChannel::~HandlerChannel() noexcept {

}

int HandlerChannel::HandleReadEvent(void* data) {
    auto input_buffer = m_conn->getInputBuffer();
    if (input_buffer->ReadBuffer(m_conn) > 0) {
        // 应用程序真正读取Buffer里的数据
        if (m_conn->m_message_handler != nullptr) {
            m_conn->m_message_handler->onMessage(*input_buffer, *m_conn);
        }
    } else {

    }
}

int HandlerChannel::HandleWriteEvent(void* data) {

}