#include "EventLoop.h"

#include <errno.h>
#include <sys/eventfd.h>

#include <memory>

#include "Channel.h"
#include "CurrentThread.h"
#include "Logger.h"
#include "Poller.h"

// __thread代表这个变量是这个线程独有的
// 防止一个线程创建多个EventLoop
__thread EventLoop* t_loopInThisThread = nullptr;

const int kPollTimeMs = 10000;

int createEventfd() {
  int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0) {
		LOG_FATAL("eventfd error: %d\n", errno);
	}
	return evtfd;
}

EventLoop::EventLoop() : looping_(false), quit_(false), callingPendingFunctors_(false), threadId_(CurrentThread::tid()), poller_(Poller::newDefaultPoller(this)), 
