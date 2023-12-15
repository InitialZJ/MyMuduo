#ifndef THREAD_H_
#define THREAD_H_

#include <unistd.h>

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include "noncopyable.h"

/**
 * @brief 只关注一个线程
 */
class Thread : muduo::noncopyable {
 public:
  using ThreadFunc = std::function<void()>;
  explicit Thread(ThreadFunc, const std::string& name = std::string());
  ~Thread();
  void start();
  void join();

  bool started() const { return started_; }
  pid_t tid() const { return tid_; }
  static int numCreated() { return numCreated_; }

 private:
  void setDefaultName();
  bool started_;
  bool joined_;
  pid_t tid_;
  ThreadFunc func_;
  std::string name_;
  // 如果直接定义个thread对象，那这个线程就直接开始运行了
  // 所以这里定义一个智能指针，在需要运行的时候再给他创建对象
  std::shared_ptr<std::thread> thread_;
  static std::atomic<int> numCreated_;
};

#endif  // !THREAD_H_
