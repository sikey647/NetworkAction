#ifndef NETWORKACTION_WAKEUPCHANNEL_H
#define NETWORKACTION_WAKEUPCHANNEL_H

#include "Channel.h"

class WakeupChannel : public Channel {
public:
    WakeupChannel(int fd);
    virtual ~WakeupChannel();
    virtual int HandleReadEvent();
    virtual int HandleWriteEvent();
};

#endif //NETWORKACTION_WAKEUPCHANNEL_H
