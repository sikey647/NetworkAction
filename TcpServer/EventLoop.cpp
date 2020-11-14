//
// Created by Sikey Liu on 2020/11/14.
//

#include "EventLoop.h"
#include "WakeupChannel.h"

EventLoop::EventLoop(std::shared_ptr<EventDispatcher> event_dispatcher) :
    m_finish(false), m_event_dispatcher(event_dispatcher) {
    m_thread_id = std::this_thread::get_id();

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_socket_pair) < 0) {
        std::cout << "socket_pair set failed" << std::endl;
        exit(-1);
    }

    m_channel_map[m_socket_pair[1]] = std::make_shared<WakeupChannel>(m_socket_pair[1]);
}

EventLoop::~EventLoop() {

}

int EventLoop::Run() {
    while (!m_finish) {
        m_event_dispatcher->Dispatch(this);
        handlePendingChanel();
    }

    return 0;
}

int EventLoop::HandleEvent(int fd, int event_type) {
    auto channel_itr = m_channel_map.find(fd);
    if (channel_itr == m_channel_map.end()) {
        std::cout << "fd: " << fd << " not in channel_map." << std::endl;
        return -1;
    }

    if (event_type & EVENT_READ) {
        channel_itr.second->HandleReadEvent();
    }

    if (event_type & EVENT_WRITE) {
        channel_itr.second->HandleWriteEvent();
    }

    return 0;
}

int EventLoop::addChannelEvent(std::shared_ptr <Channel> channel) {
    makeChannelEvent(channel, CHANNEL_OPS::ADD);
}

int EventLoop::delChannelEvent(std::shared_ptr <Channel> channel) {
    makeChannelEvent(channel, CHANNEL_OPS::DEL);
}

int EventLoop::modChannelEvent(std::shared_ptr <Channel> channel) {
    makeChannelEvent(channel, CHANNEL_OPS::MOD);
}

int EventLoop::makeChannelEvent(std::shared_ptr <Channel> channel, int channel_ops) {
    std::unique_lock<std::mutex> lock(m_mtx);
    m_pending_channels.push(std::make_pair<int, std::shared_ptr<Channel>>(channel_ops, channel));
    lock.unlock();

    if (std::this_thread::get_id() == m_thread_id) {
        handlePendingChanel();
    } else {
        // wakeup sub thread
    }
}

int EventLoop::handlePendingChanel() {
    std::unique_lock<std::mutex> lock(m_mtx);
    int size = m_pending_channels.size();
    for (int i = 0; i < size; i++) {
        auto pending_channel = m_pending_channels.front();
        m_pending_channels.pop();
        if (pending_channel.first == CHANNEL_OPS::ADD) {
            addPendingChannel(pending_channel.second);
        } else if (pending_channel.first == CHANNEL_OPS::DEL) {
            delPendingChannel(pending_channel.second);
        } else if (pending_channel.first == CHANNEL_OPS::MOD) {
            modPendingChannel(pending_channel.second);
        }
    }

    return 0;
}

int EventLoop::addPendingChannel(std::shared_ptr<Channel> channel) {
    int fd = channel->getFd();

    if (m_channel_map.find(fd) == m_channel_map.end()) {
        m_channel_map[fd] = channel;
        m_event_dispatcher->Add(channel);
    }

    return 0;
}

int EventLoop::delPendingChannel(std::shared_ptr <Channel> channel) {
    int fd = channel->getFd();

    if (m_channel_map.find(fd) != m_channel_map.end()) {
        m_event_dispatcher->Del(channel);
        m_channel_map.erase(fd);
    }

    return 0;
}

int EventLoop::modPendingChannel(std::shared_ptr <Channel> channel) {
    int fd = channel->getFd();

    if (m_channel_map.find(fd) != m_channel_map.end()) {
        m_channel_map[fd] = channel;
        m_event_dispatcher->Mod(channel);
    }

    return 0;
}