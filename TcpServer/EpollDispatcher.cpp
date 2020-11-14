//
// Created by Sikey Liu on 2020/11/14.
//

#include <stdlib.h>
#include <sys/epoll.h>
#include <iostream>
#include "EpollDispatcher.h"

EpollDispatcher::EpollDispatcher(int timeout = -1) : m_timeout(timeout) {
    m_efd = epoll_create1(0);
    if (m_efd == -1) {
        perror("epoll_create1");
        std::cout << "init EpollDispatcher failed!" << std::endl;
    }

    m_events = calloc(MAX_EVENTS, sizeof(struct epoll_event));
}

virtual ~EpollDispatcher() {

}

int EpollDispatcher::Add(std::shared_ptr<Channel> channel) {
    int fd = channel->getFd();
    int event_type = getEventType();

    int events = 0;
    if (event_type & EVENT_READ) {
        events = events | EPOLLIN;
    }
    if (event_type & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if (epoll_ctl(m_efd, EPOLL_CTL_ADD, fd, &event) == -1) {
        perror("epoll_ctl()");
        std::cout << "EpollDispatcher Add failed!" << std::endl;
        return -1;
    }

    return 0;
}

int EpollDispatcher::Del(std::shared_ptr<Channel> channel) {
    int fd = channel->getFd();
    int event_type = getEventType();

    int events = 0;
    if (event_type & EVENT_READ) {
        events = events | EPOLLIN;
    }
    if (event_type & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    if (epoll_ctl(m_efd, EPOLL_CTL_DEL, fd, &event) == -1) {
        perror("epoll_ctl()");
        std::cout << "EpollDispatcher Del failed!" << std::endl;
        return -1;
    }

    return 0;
}

int EpollDispatcher::Mod(std::shared_ptr<Channel> channel) {
    int fd = channel->getFd();
    int event_type = getEventType();

    int events = 0;
    if (event_type & EVENT_READ) {
        events = events | EPOLLIN;
    }
    if (event_type & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    if (epoll_ctl(m_efd, EPOLL_CTL_MOD, fd, &event) == -1) {
        perror("epoll_ctl()");
        std::cout << "EpollDispatcher Mod failed!" << std::endl;
        return -1;
    }

    return 0;
}

int EpollDispatcher::Dispatch(EventLoop *event_loop) {
    int n = epoll_wait(m_efd, m_events, MAX_EVENTS, m_timeout);
    for (int i = 0; i < n; i++) {
        if ((m_events[i].events & EPOLLERR) || (m_events[i].events & EPOLLHUP)) {
            std::cout << "epoll error" << std::endl;
            clock(m_events[i].data.fd);
            continue;
        }

        if (m_events[i].events & EPOLLIN) {
            event_loop->HandleEvent(m_events[i].data.fd, EVENT_READ);
        }

        if (m_events[i].events & EPOLLOUT) {
            event_loop->HandleEvent(m_events[i].data.fd, EVENT_WRITE);
        }
    }

    return 0;
}