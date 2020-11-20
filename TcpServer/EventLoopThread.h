#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "EventLoop.h"

class EventLoopThread {  
  public:
    EventLoopThread() {}
    virtual ~EventLoopThread() {}  
    void Start();
    void run();
    EventLoop* getEventLoop() { 
        return m_event_loop.get(); 
    }

  private:
    std::shared_ptr<EventLoop> m_event_loop;
    std::thread m_thread;
    std::mutex m_mtx;
    std::condition_variable m_cond;
};
