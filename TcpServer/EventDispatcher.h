#ifndef NETWORKACTION_EVENTDISPATCHER_H
#define NETWORKACTION_EVENTDISPATCHER_H

#include <unordered_map>
#include <memory>
#include "EventLoop.h"
#include "Channel.h"

class EventDispatcher {
public:
    EventDispatcher() {};
    virtual ~EventDispatcher() {};
    virtual int Add(std::shared_ptr<Channel> channel) = 0;
    virtual int Del(std::shared_ptr<Channel> channel) = 0;
    virtual int Mod(std::shared_ptr<Channel> channel) = 0;
    virtual int Dispatch(EventLoop *event_loop) = 0;
};


#endif //NETWORKACTION_EVENTDISPATCHER_H
