## 设计需求
* 采用 reactor 模型，可以灵活使用 poll/epoll 作为事件分发实现
* 支持多线程
  * 支持单线程单 reactor 模式
  * 支持多线程主 - 从 reactor 模式
  * 支持将套接字上的 I/O 事件分离到多个工作线程上
* 封装读写操作到 Buffer 对象中

## 设计思路
### 反应堆模式设计
基于事件分发和回调的反应堆框架
* event_loop
  * 一个线程绑定的无限事件循环
  * 一个无限循环着的事件分发器，一旦有事件发生，它就会回调预先定义好的回调函数，完成事件的处理
  * 可以使用 poll 或者 epoll 方法将一个线程阻塞，等待各种 I/O 事件的发生
  ```c++
  class EventLoop {  
  public:
      EventLoop(int timeout) : m_timeout(timeout) {}
      virtual ~EventLoop() {}
      int Run();
  
  private:
      int addEvent(int fd, Event *event);
      int delEvent(int fd, Event *event);
      int update(int fd, Event *event);
      int handleEvent();  // ?????
      int handleWakeup();
         
  private:
      bool m_finish;
      int m_timeout;
      EventDispatcher *m_event_dispatcher;
      unordered_map<int, Event> m_event_map;
      int m_socket_pair[2];
      std::mutex m_mtx;
      std::condition_variable m_cond;
  };
  
  int EventLoop::Run() {  
      struct timeval timeval;
      timeval.tv_sec = m_timeout;
  
      while (!m_finish) {
          m_event_dispatcher->dispatch(m_timeout);
          handle_event();
      }
  
      return 0;
  }
  ```
* event
  * 对各种注册到 event_loop 上的各种时间对象的抽象
  ```c++
  class Event {
  public:  
      Event(int fd, int type) {}
      virtual ~Event() {}
  
  public:
      int HandleReadEvent();
      int HandleWriteEvent();
  
  private:
      int m_fd;
      int m_type;        
  };
  ```
* acceptor
  * 表示服务器端监听器
  * 会作为一个 event 对象，注册到 event_loop 上，以便进行连接完成的事件分发和检测
* event_dispatcher
  * 对事件分发机制的一种抽象
  * 可以实现一个基于 poll 的 poll_dispatcher，也可以实现一个基于 epoll 的 epoll_dispatcher
  ```c++
  class EventDispatcher {  
  public:
      EventDispatcher() {}
      virtual ~EventDispatcher() {}
  
  public:
      int Add(Event *event);
      int Del(Event *event);
      int Update(Event *event);
      int Dispatch(struct timeval *timeout);
  };
  ```
* event_map
  * 保存了描述字到 event 的映射
  * 在事件发生时，根据事件类型对应的套接字快速找到 event 对象里的事件处理函数

### I/O 模型和多线程模型设计
解决 event_loop 的线程运行问题，以及事件分发和回调的线程执行问题
* thread_pool
  * 维护了一个 sub-reactor 的线程列表，可以提供给主 reactor 线程使用
  * 每次当有新的连接建立时，可以从 thread_pool 里获取一个线程，以便用它来完成对新连接套接字的 read/write 事件注册，将 I/O 线程和主 reactor 线程分离
  ```c++
  class ThreadPool {
  public:
      ThreadPool(EventLoop *event_loop, int thread_num) : 
          m_main_event_loop(event_loop), 
          m_thread_num(m_thread_num) {}
      virtual ~ThreadPool() {}
      
      void Start();
  
  private:
      EventLoop *m_main_event_loop;
      bool m_start;
      int m_thread_num;
      vector<EventLoopThread> m_event_loop_threads;
      int m_pos;
  };
  
  void Start() {  
      m_start(true);
      if (m_thread_num <= 0)
          return ;
      m_event_loop_threads.resize(m_thread_num);
      for (int i = 0; i < m_thread_num; i++) {
          m_event_loop_threads[i].Start();
      }
  }
  
  EventLoop *getEventLoop() {
      if (!m_start)
          return nullptr;
      
      EventLoop *selected = m_main_event_loop;
      if (m_thread_num > 0) {
          selected = m_event_loop_threads[m_pos].getEventLoop();
          if (++m_pos >= m_thread_num) {
              m_pos = 0;
          }
      }
  
      return selected;
  }
  ``` 
* event_loop_thread
  * reactor 的线程实现
  * 连接套接字的 read/write 事件检测都是在这个线程里完成
  ```c++
  class EventLoopThread {  
  public:
      EventLoopThread() {}
      virtual ~EventLoopThread() {}    
  
  private:
      EventLoop *m_event_loop;
      std::thread m_thread;
      std::mutex m_mtx;
      std::condition_variable m_cond;
  };
  
  void EventLoopThread::Start() {
      m_thread = std::thread(&this->run);
      std::unique_lock<std::mutex> lock(m_mtx);
      while (m_event_loop == nullptr) {
         m_cond.wait(lock); 
      } 
  }
  
  void EventLoopThread::run() {
      std::unique_lock<std::mutex> lock(m_mtx);
      m_event_loop = new EventLoop();
      m_cond.notify_one();
      lock.unlock();
      m_event_loop->Run();
  }
  ```
#### 思考
* main reactor 线程是一个 acceptor 线程，这个线程一旦创建，会以 event_loop 形式阻塞在 event_dispatcher 的 dispatch 方法上 
* 它在等待监听套接字上的事件发生，也就是已完成的连接，一旦有连接完成，就会创建出连接对象 tcp_connection，以及 event 对象等
* 如果使用多个 sub-reactor 子线程时，主线程会创建多个子线程，每个子线程在创建之后，按照主线程指定的启动函数立即运行，并进行初始化

#### 问题-1
* 主线程如何判断子线程已经完成初始化并启动，继续执行下去呢？
* main reactor 线程等待多个 sub-reactor 子线程初始化完
  * 
#### 问题-2
* main-reactor 线程通知 sub-reactor 线程有新的事件加入，而这个 sub-reactor 线程很可能是处于事件分发的阻塞调用之中，如何协调 main-reactor 线程数据写入给 sub-reactor 线程？



### Buffer 和数据读写
* buffer
  * 屏蔽了对套接字进行的写和读的操作，用来表示从连接套接字收取的数据，以及应用程序即将需要发送出去的数据
* tcp_connection
  * 表示已建立的 TCP 连接
  * 属性包括接收缓冲区、发送缓冲区、event 对象等