#ifndef NETWORKACTION_EVENT_H
#define NETWORKACTION_EVENT_H

enum EVENT_TYPE {
    READ,
    WRITE
};

class Event {
public:
    Event(int fd, EVENT_TYPE event_type):m_event_fd(fd),m_event_type(event_type) {};
    ~Event() {};
    int getFd() { return m_event_fd; }
    int getEventType() { return m_event_type; }

    virtual void work() = 0;

private:
    int m_event_fd;
    EVENT_TYPE m_event_type;
};


#endif //NETWORKACTION_EVENT_H
