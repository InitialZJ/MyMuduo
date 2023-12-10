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
	// 当
	int wakeupFd_;

};

#endif  // !EVENT_LOOP_H_
