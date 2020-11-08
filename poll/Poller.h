#ifndef NETWORKACTION_POLLER_H
#define NETWORKACTION_POLLER_H

#include <poll.h>
#include "EventLoop.h"
#include "Event.h"

const int INIT_SIZE = 128;

class Poller : EventLoop {
public:
    Poller();
    virtual ~Poller();

    virtual void Register(std::shared_ptr<Event> event);
    virtual void UnRegister(std::shared_ptr<Event> event);
    virtual void Run();

protected:
    virtual bool Ready(std::shared_ptr<Event> event);

private:
    struct pollfd event_set[INIT_SIZE];
};


#endif //NETWORKACTION_POLLER_H
