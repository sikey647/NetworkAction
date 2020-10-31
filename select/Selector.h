#ifndef NETWORKACTION_SELECTOR_H
#define NETWORKACTION_SELECTOR_H

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <map>
#include <memory>
#include "Event.h"

class Selector {
    Selector();
    ~Selector();

    void Register(std::shared_ptr<Event> event);
    void UnRegister(std::shared_ptr<Event> event);
    bool Ready(std::shared_ptr<Event> event);
    void Run();

private:
    fd_set m_read_mask;
    fd_set m_all_reads;
    std::map<int, std::shared_ptr<Event>> m_events;
    int m_status;
};


#endif //NETWORKACTION_SELECTOR_H
