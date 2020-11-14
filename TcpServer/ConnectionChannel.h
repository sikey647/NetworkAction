#ifndef NETWORKACTION_CONNECTIONCHANNEL_H
#define NETWORKACTION_CONNECTIONCHANNEL_H

#include "Channel.h"

class ConnectionChannel : public Channel {
public:
    ConnectionChannel(int listen_fd);
    ~ConnectionChannel();
    virtual int HandleReadEvent();
    virtual int HandleWriteEvent();

private:
    void setNonblocking(int fd);
};


#endif //NETWORKACTION_CONNECTIONCHANNEL_H
