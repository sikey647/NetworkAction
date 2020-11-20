#ifndef NETWORKACTION_CONNECTIONCHANNEL_H
#define NETWORKACTION_CONNECTIONCHANNEL_H

#include "Channel.h"
#include "EventLoop.h"

class ConnectionChannel : public Channel {
public:
    ConnectionChannel(int listen_fd, EventLoop *event_loop);
    virtual ~ConnectionChannel();
    virtual int HandleReadEvent(void* data);
    virtual int HandleWriteEvent(void* data);

private:
    void setNonblocking(int fd);

    EventLoop *m_event_loop;
};


#endif //NETWORKACTION_CONNECTIONCHANNEL_H
