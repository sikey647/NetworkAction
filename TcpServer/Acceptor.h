#ifndef NETWORKACTION_ACCEPTOR_H
#define NETWORKACTION_ACCEPTOR_H

const int LISTENQ = 1024;

class Acceptor {
public:
    Acceptor(int port);
    ~Acceptor();
    int getListenFd();

private:
    int m_port;
    int m_listen_fd;
};


#endif //NETWORKACTION_ACCEPTOR_H
