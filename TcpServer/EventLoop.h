#ifndef NETWORKACTION_EVENTLOOP_H
#define NETWORKACTION_EVENTLOOP_H

#include <memory>
#include <unordered_map>
#include <queue>
#include <utility>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "EventDispatcher.h"
#include "Channel.h"

enum CHANNEL_OPS {
    ADD,
    DEL,
    MOD
};

class EventLoop {
public:
    EventLoop(std::shared_ptr<EventDispatcher> event_dispatcher);
    virtual ~EventLoop();
    int Run();
    int HandleEvent(int fd, int event_type);

    int addChannelEvent(std::shared_ptr<Channel> channel);
    int delChannelEvent(std::shared_ptr<Channel> channel);
    int modChannelEvent(std::shared_ptr<Channel> channel);

private:
    int makeChannelEvent(std::shared_ptr<Channel> channel, int channel_ops);
    int handlePendingChanel();
    int addPendingChannel(std::shared_ptr<Channel> channel);
    int delPendingChannel(std::shared_ptr<Channel> channel);
    int modPendingChannel(std::shared_ptr<Channel> channel);

    int handleWakeup();

private:
    //
    std::thread::id m_thread_id;

    // 事件循环的条件
    bool m_finish;

    // select、poll、epoll实现
    std::shared_ptr<EventDispatcher> m_event_dispatcher;

    // key: conn_fd, value: Channel
    std::unordered_map<int, std::shared_ptr<Channel>> m_channel_map;

    // 用于 MainThread 与 SubThread 通信
    // MainThread使用[0]，注册写事件，向SubThread发送消息
    // SubThread使用[1]，注册读事件，接收MainThread的消息
    int m_socket_pair[2];

    // 待处理事件
    std::queue<std::pair<int, std::shared_ptr<Channel>>> m_pending_channels;
    //bool m_handle_pending;

    // 用于 MainThread 与 SubThread 同步
    std::mutex m_mtx;
    std::condition_variable m_cond;
};


#endif //NETWORKACTION_EVENTLOOP_H
