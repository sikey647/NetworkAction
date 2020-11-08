#ifndef NETWORKACTION_READLINEEVENT_H
#define NETWORKACTION_READLINEEVENT_H
#include "Event.h"

const int MAX_LINE = 1024;

class ReadLineEvent : public Event {
public:
    ReadLineEvent(int fd, int send_socket_fd) : Event(fd), m_send_socket_fd(send_socket_fd){}
    ~ReadLineEvent() {};
    virtual void work() = 0;

private:
    int m_send_socket_fd;
    char m_send_line[MAXLINE];
};


#endif //NETWORKACTION_READLINEEVENT_H
