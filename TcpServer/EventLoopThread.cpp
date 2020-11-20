 #include "EventLoopThread.h"
 
void EventLoopThread::Start() {
    m_thread = std::thread(&EventLoopThread::run, this);
    std::unique_lock<std::mutex> lock(m_mtx);
    while (m_event_loop == nullptr) {
        m_cond.wait(lock); 
    } 
}

void EventLoopThread::run() {
    std::unique_lock<std::mutex> lock(m_mtx);
    m_event_loop = std::make_shared<EventLoop>();
    m_cond.notify_one();
    lock.unlock();
    m_event_loop->Run();
}