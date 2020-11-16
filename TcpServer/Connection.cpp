#include "Connection.h"

Connection::Connection() :
    m_connection_completed_handler(connection_completed_handler),
    m_message_handler(message_handler),
    m_write_completed_handler(m_write_completed_handler),
    m_connection_closed_handler(connection_closed_handler) {
    m_input_buffer = std::make_shared<Buffer>();
    m_output_buffer = std::make_shared<Buffer>();

    // make channel


    if (m_connection_completed_handler != nullptr) {
        m_connection_completed_handler->onConnectionCompleted(*this);
    }
}

int Connection::closeConnection() {

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

