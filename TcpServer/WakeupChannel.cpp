//
// Created by Sikey Liu on 2020/11/14.
//

#include "WakeupChannel.h"

WakeupChannel::WakeupChannel(int fd) : m_fd(fd) {
    m_event_type = EVENT_READ;
}

WakeupChannel::~WakeupChannel() {

}

int WakeupChannel::HandleReadEvent() {
    char one;
    ssize_t n = read(m_fd, &one, sizeof(one));
    if (n != sizeof(one)) {
        std::cout << "Wakeup failed!" << std::endl;
    }
}

int WakeupChannel::HandleWriteEvent() {

}