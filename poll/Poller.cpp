#include "Poller.h"

Poller::Poller() {
    for (int i = 0; i < INIT_SIZE; i++) {
        event_set[i].fd = -1;
    }
}

Poller::~Poller() noexcept {

}

void Poller::Register(std::shared_ptr<Event> event) {
    for (int i = 0; i < INIT_SIZE; i++) {
        if (event_set[i].fd < 0) {
            event_set[i].fd = event->getFd();
            if (event->getEventType() == EVENT_TYPE::READ) {
                event_set[i].events = POLLRDNORM;
            } else if (event->getEventType() == EVENT_TYPE::WRITE) {
                event_set[i].events = POLLWRNORM;
            }
            return;
        }
    }

    error(1, errno, "can not register so many clients");
}

void Poller::UnRegister(std::shared_ptr <Event> event) {
    for (int i = 0; i < INIT_SIZE; i++) {

    }
}