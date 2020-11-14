#ifndef NETWORKACTION_EPOLLDISPATCHER_H
#define NETWORKACTION_EPOLLDISPATCHER_H

#include <unordered_map>
#include <memory>
#include "EventDispatcher.h"

const int MAX_EVENTS = 128;

class EpollDispatcher : public EventDispatcher {
public:
    EpollDispatcher(int timeout = -1);
    virtual ~EpollDispatcher();

    virtual int Add(std::shared_ptr<Channel> channel);
    virtual int Del(std::shared_ptr<Channel> channel);
    virtual int Mod(std::shared_ptr<Channel> channel);
    virtual int Dispatch(EventLoop *event_loop);

private:
    int m_efd;
    struct epoll_event *m_events;
    int m_timeout;
};


#endif //NETWORKACTION_EPOLLDISPATCHER_H
