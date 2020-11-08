//
// Created by Sikey Liu on 2020/11/2.
//

#ifndef NETWORKACTION_EVENTLOOP_H
#define NETWORKACTION_EVENTLOOP_H

#include <map>
#include <memory>
#include "Event.h"

class EventLoop {
public:
    EventLoop();
    virtual ~EventLoop();

    virtual void Register(std::shared_ptr<Event> event) = 0;
    virtual void UnRegister(std::shared_ptr<Event> event) = 0;
    virtual void Run() = 0;

protected:
    virtual bool Ready(std::shared_ptr<Event> event) = 0;

private:
    std::map<int, std::shared_ptr<Event>> m_events;
    int m_status;
};

#endif //NETWORKACTION_EVENTLOOP_H
