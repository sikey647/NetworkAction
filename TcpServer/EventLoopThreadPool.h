#include <vector>

#include "EventLoop.h"
#include "EventLoopThread.h"

class EventLoopThreadPool {
  public:
      EventLoopThreadPool(EventLoop *main_event_loop, int thread_num);
      virtual ~EventLoopThreadPool();
      void Start();
      EventLoop* getEventLoop();
  
  private:
      EventLoop *m_main_event_loop;
      bool m_start;
      int m_thread_num;
      std::vector<EventLoopThread> m_event_loop_threads;
      int m_pos;
  };
  
