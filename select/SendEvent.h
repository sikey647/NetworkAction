//
// Created by Sikey Liu on 2020/10/31.
//

#ifndef NETWORKACTION_SENDEVENT_H
#define NETWORKACTION_SENDEVENT_H
#include "Event.h"

const int MAX_LINE = 1024;

class SendEvent : public Event {
public:
    SendEvent(int fd) : Event(fd) {}
    ~SendEvent() {};
    virtual void work() = 0;

private:

};


#endif //NETWORKACTION_SENDEVENT_H
