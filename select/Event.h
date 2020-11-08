#ifndef NETWORKACTION_EVENT_H
#define NETWORKACTION_EVENT_H


class Event {
public:
    Event(int fd):m_event_fd(fd) {};
    ~Event() {};
    int getFd() { return m_event_fd; }
    virtual void work() = 0;

private:
    int m_event_fd;
};


#endif //NETWORKACTION_EVENT_H
