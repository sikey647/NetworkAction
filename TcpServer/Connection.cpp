#include "Connection.h"

Connection::Connection(int conn_fd,
    shared_ptr<ConnectionCompletedHandler> connection_completed_handler,
    std::shared_ptr<MessageHandler> message_handler,
    std::shared_ptr<WriteCompletedHandler> write_completed_handler,
    std::shared_ptr<ConnectionClosedHandler> connection_closed_handler,
    EventLoop *event_loop) :
    m_conn_fd(conn_fd),
    m_connection_completed_handler(connection_completed_handler),
    m_message_handler(message_handler),
    m_write_completed_handler(write_completed_handler),
    m_connection_closed_handler(connection_closed_handler),
    m_event_loop(event_loop) {
    m_input_buffer = std::make_shared<Buffer>();
    m_output_buffer = std::make_shared<Buffer>();

    // make channel
    createChannel();
}

int Connection::shutdownConnection() {
    if (shutdown(m_channel->getFd(), SHUT_WR) < 0) {
        std::count << "Connection shutdown failed, socket = " << m_channel->getFd() << std::endl;
    }
}

std::shared_ptr<Channel> Connection::getChannel() {
    return m_channel;
}

std::shared_ptr<Buffer> Connection::getInputBuffer() {
    return m_input_buffer;
}

std::shared_ptr<Buffer> Connection::getOutputBuffer() {
    return m_output_buffer;
}

Connection::~Connection() {

}

int Connection::createChannel() {
    m_channel = std::make_shared<HandlerChannel>(m_conn_fd, this);
    m_event_loop->addChannelEvent(m_channel);
    if (m_connection_completed_handler != nullptr) {
        m_connection_completed_handler->onConnectionCompleted(*this);
    }
}

int Connection::closeChannel() {
    m_event_loop->delChannelEvent(m_channel);
    if (m_connection_closed_handler != nullptr) {
        m_connection_closed_handler->onConnectionClosed(*this);
    }
}

int Connection::onChannelMessage() {
    if (m_message_handler != nullptr) {
        m_message_handler->onMessage(*m_input_buffer, *this);
    }
}

int Connection::readChannelBuffer() {
    return m_input_buffer->ReadBuffer(m_conn_fd);
}

int Connection::writeChannelBuffer() {
    ssize_t n_write = write(m_conn_fd, m_output_buffer->getData + m_output_buffer->getReadIndex,
                            m_output_buffer->getReadableSize());
    if (n_write > 0) {
        //已读 n_write 字节
        m_output_buffer->getReadIndex += n_write;
        //如果数据完全发送出去，就不需要继续了
        if (m_output_buffer->getReadableSize() == 0) {
            m_channel->disableWriteEvent(channel);   // ???????
        }
        //回调writeCompletedCallBack
        if (m_write_completed_handler != nullptr) {
            m_write_completed_handler->onWriteCompleted(*this);
        }
    }
}
