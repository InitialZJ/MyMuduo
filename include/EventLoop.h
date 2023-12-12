#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "CurrentThread.h"
#include "Timestamp.h"
#include "noncopyable.h"

class Channel;
class Poller;

class EventLoop : public muduo::noncopyable {
 public:
  using Functor = std::function<void>();
  EventLoop();
  ~EventLoop();
  void loop();  // 开启事件循环
  void quit();  // 关闭事件循环
  TimeStamp poolReturnTime() const { return pollReturnTime_; }
  void runInLoop(Functor cb);  // mainReactor用于唤醒subReactor的
  void queueInLoop(Functor cb);
  void wakeup();
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);
  void hasChannel(Channel* channel);

  // 判断当前的eventLooop对象是否在自己的线程里面
  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

 private:
  // 处理唤醒相关的逻辑
  void handleRead();
  // 执行回调
  void doPendingFunctors();

  using ChannelList = std::vector<Channel*>;
  std::atomic<bool> looping_;
  std::atomic<bool> quit_;
  std::atomic<bool> callingPendingFunctors_;
  const pid_t threadId_;
  TimeStamp pollReturnTime_;
  std::unique_ptr<Poller> poller_;
  // 当mainLoop获取一个新用户的channel时通过轮询算法选择一个subLoop(subReactor)来处理channel
  int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;
  ChannelList activateChannels_;
  Channel* currentActivateChannel_;
  // 存储Loop需要执行的所有回调操作
  std::vector<Functor> pendingFunctors_;
  std::mutex mutex_;
};

#endif  // !EVENT_LOOP_H_
