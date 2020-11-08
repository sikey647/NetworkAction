#include "Selector.h"

Selector::Selector() {
    FD_ZERO(&m_all_reads);
    FD_ZERO(&m_read_mask);
    m_status = 1;
}

Selector::~Selector() {

}

void Selector::Register(std::shared_ptr<Event> event) {
    int event_fd = event->getFd();
    FD_SET(event_fd, &m_all_reads);
    m_events[event_fd] = event;
}

void Selector::UnRegister(std::shared_ptr<Event> event) {
    int event_fd = event->getFd();
    if (m_events.find(event_fd) != m_events.end()) {
        m_events.erase(event_fd);
        FD_CLR(fd, &m_all_reads);
    }
}

bool Selector::Ready(std::shared_ptr <Event> event) {
    int event_fd = event->getFd();
    return FD_ISSET(event_fd, &m_read_mask);
}

void Selector::Run() {
    while (m_status) {
        m_read_mask = m_all_reads;
        int max_fd = m_events.end()->first;
        int ret = select(max_fd + 1, &m_read_mask, NULL, NULL, NULL);
        if (ret <= 0) {
            error(stdout, errno, "select failed!");
        }

        for (auto &itr_event : m_events) {
            if (Ready(itr_event.second)) {
                itr_event.second->work();
            }
        }
    }
}