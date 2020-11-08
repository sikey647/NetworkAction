#ifndef NETWORKACTION_SELECTOR_H
#define NETWORKACTION_SELECTOR_H

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <map>
#include <memory>
#include "EventLoop.h"
#include "Event.h"


class Selector : public EventLoop {
    Selector();
    ~Selector();

    void Register(std::shared_ptr<Event> event);
    void UnRegister(std::shared_ptr<Event> event);
    bool Ready(std::shared_ptr<Event> event);
    void Run();

private:
    fd_set m_read_mask;
    fd_set m_all_reads;
};

#endif //NETWORKACTION_SELECTOR_H
