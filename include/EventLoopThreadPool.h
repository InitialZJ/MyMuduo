#ifndef EVENT_LOOP_THREAD_POOL_H_
#define EVENT_LOOP_THREAD_POOL_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "noncopyable.h"

class EventLoop;
class EventLoopThread;
class EventLoopThreadPool : public muduo::noncopyable {
 public:
  using ThreadInitCallback = std::function<void(EventLoop*)>;
  EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg);
  ~EventLoopThreadPool();
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }
  void start(const ThreadInitCallback& cb = ThreadInitCallback());

  EventLoop* getNextLoop();
  std::vector<EventLoop*> getAllGroups();
  bool started() const { return started_; }
  const std::string name() const { return name_; }

 private:
  EventLoop* baseLoop_;
  std::string name_;
  bool started_;
  int numThreads_;
  int next_;
  std::vector<std::unique_ptr<EventLoopThread>> threads_;
  std::vector<EventLoop*> loops_;
};

#endif  // !EVENT_LOOP_THREAD_POOL_H_
