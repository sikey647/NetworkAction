#ifndef NETWORKACTION_HANDLERCHANNEL_H
#define NETWORKACTION_HANDLERCHANNEL_H

#include "Channel.h"
#include "Connection.h"

class HandlerChannel : public Channel {
public:
    HandlerChannel(int conn_fd, Connection *conn);
    virtual ~HandlerChannel();
    virtual int HandleReadEvent(void* data);
    virtual int HandleWriteEvent(void* data);

private:
    Connection *m_conn;
};

#endif //NETWORKACTION_HANDLERCHANNEL_H