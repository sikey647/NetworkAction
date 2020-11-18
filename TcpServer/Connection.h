#ifndef NETWORKACTION_CONNECTION_H
#define NETWORKACTION_CONNECTION_H

#include <memory>

#include "EventLoop.h"
#include "Channel.h"
#include "Buffer.h"
#include "HandlerChannel.h"

class ConnectionCompletedHandler {
public:
    virtual ~ConnectionCompletedHandler() {};
    virtual int onConnectionCompleted(const Connection &conn) = 0;
};

class MessageHandler {
public:
    virtual ~MessageHandler() {};
    virtual int onMessage(const Buffer &input_buffer, const Connection &conn) = 0;
};

class WriteCompletedHandler {
public:
    virtual ~WriteCompletedHandler() {};
    virtual int onWriteCompleted(const Connection &conn) = 0;
};

class ConnectionClosedHandler {
public:
    virtual ~ConnectionClosedHandler() {};
    virtual int onConnectionClosed(const Connection &conn) = 0;
};

class Connection {
public:
    Connection(int conn_fd,
               shared_ptr<ConnectionCompletedHandler> connection_completed_handler,
               std::shared_ptr<MessageHandler> message_handler,
               std::shared_ptr<WriteCompletedHandler> write_completed_handler,
               std::shared_ptr<ConnectionClosedHandler> connection_closed_handler,
               EventLoop *event_loop);
    virtual ~Connection();
    std::shared_ptr<Channel> getChannel();
    std::shared_ptr<Buffer> getInputBuffer();
    std::shared_ptr<Buffer> getOutputBuffer();
    int shutdownConnection();
    int removeChannel();
    int onChannelMessage();
    int readChannelBuffer();
    int writeChannelBuffer();
private:
    int createChannel();

private:
    int m_conn_fd;

    // 处理事件
    std::shared_ptr<HandlerChannel> m_channel;

    // 接收缓冲区
    std::shared_ptr<Buffer> m_input_buffer;
    // 发送缓冲区
    std::shared_ptr<Buffer> m_output_buffer;

    // 回掉函数
    std::shared_ptr<ConnectionCompletedHandler> m_connection_completed_handler;
    std::shared_ptr<MessageHandler> m_message_handler;
    std::shared_ptr<WriteCompletedHandler> m_write_completed_handler;
    std::shared_ptr<ConnectionClosedHandler> m_connection_closed_handler;

    EventLoop *m_event_loop;
};


#endif //NETWORKACTION_CONNECTION_H
