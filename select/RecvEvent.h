#ifndef NETWORKACTION_RECVEVENT_H
#define NETWORKACTION_RECVEVENT_H

#include "Event.h"

const int MAX_LINE = 1024;

class RecvEvent : public Event {
public:
    RecvEvent(int fd) : Event(fd) {}
    ~RecvEvent();
    virtual void work() = 0;

private:
    char m_recv_line[MAX_LINE];
};


#endif //NETWORKACTION_RECVEVENT_H
