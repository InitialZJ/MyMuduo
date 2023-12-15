#ifndef EVENT_LOOP_THREAD_H_
#define EVENT_LOOP_THREAD_H_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>

#include "Thread.h"
#include "noncopyable.h"

class EventLoop;
class EventLoopThread : public muduo::noncopyable {
 public:
  using ThreadInitCallback = std::function<void(EventLoop*)>;
  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                  const std::string& name = std::string());
  ~EventLoopThread();
  EventLoop* startLoop();

 private:
  void threadFunc();
  EventLoop* loop_;
  bool existing_;
  Thread thread_;
  std::mutex mutex_;
  std::condition_variable cond_;
  ThreadInitCallback callback_;
};

#endif  // !EVENT_LOOP_THREAD_H_
