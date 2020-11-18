#ifndef NETWORKACTION_CONNECTIONCHANNEL_H
#define NETWORKACTION_CONNECTIONCHANNEL_H

#include "Channel.h"

class ConnectionChannel : public Channel {
public:
    ConnectionChannel(int listen_fd);
    virtual ~ConnectionChannel();
    virtual int HandleReadEvent(void* data);
    virtual int HandleWriteEvent(void* data);

private:
    void setNonblocking(int fd);

};


#endif //NETWORKACTION_CONNECTIONCHANNEL_H
