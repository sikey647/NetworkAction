#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *event_loop, int thread_num) : 
        m_main_event_loop(event_loop), 
        m_thread_num(thread_num) {

}

EventLoopThreadPool::~EventLoopThreadPool() {

}

void EventLoopThreadPool::Start() {  
    m_start = true;
    if (m_thread_num <= 0) return ;
        
    m_event_loop_threads.resize(m_thread_num);
    for (int i = 0; i < m_thread_num; i++) {
        m_event_loop_threads[i].Start();
    }
}

EventLoop* EventLoopThreadPool::getEventLoop() {
    if (!m_start) return nullptr;
    
    EventLoop *selected = m_main_event_loop;
    if (m_thread_num > 0) {
        selected = m_event_loop_threads[m_pos].getEventLoop();
        if (++m_pos >= m_thread_num) m_pos = 0;
    }

    return selected;
}

