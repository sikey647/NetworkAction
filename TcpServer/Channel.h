#ifndef NETWORKACTION_CHANNEL_H
#define NETWORKACTION_CHANNEL_H

enum EVENT {
    TIMEOUT = 0x01,
    READ = 0x02,
    WRITE = 0x04,
    SIGNAL = 0x08
};

class Channel {
public:
    Channel(int fd, int event_type) :  {}
    virtual ~Channel() {}
    virtual int HandleReadEvent() = 0;
    virtual int HandleWriteEvent() = 0;
    int getFd() {
        return m_fd;
    }

    int getEventType() {
        return m_event_type;
    }

protected:
    int m_fd;
    int m_event_type;
};

#endif //NETWORKACTION_CHANNEL_H
