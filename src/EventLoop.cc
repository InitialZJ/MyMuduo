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

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      callingPendingFunctors_(false),
      threadId_(CurrentThread::tid()),
      poller_(Poller::newDefaultPoller(this)),
      wakeupFd_(createEventfd()),
      wakeupChannel_(new Channel(this, wakeupFd_)),
      currentActivateChannel_(nullptr) {
  LOG_DEBUG("EventLoop created %p in thread %d\n", this, threadId_);
  if (t_loopInThisThread) {
    // 如果当前进程已经绑定了某个EventLoop对象，则无法创建
    LOG_FATAL("Another EventLloop %p exists in this thread %d\n",
              t_loopInThisThread, threadId_);
  } else {
    t_loopInThisThread = this;
  }
  // FIXME: 这里感觉有问题，readcallback需要传入TimeStamp
  wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
  wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
  wakeupChannel_->disableAll();
  wakeupChannel_->remove();
  close(wakeupFd_);
  t_loopInThisThread = nullptr;
}

void EventLoop::handleRead() {
  uint64_t one = 1;
  // mainReactor给subReactor发消息，subReactor通过wakeupFd_感知
  ssize_t n = read(wakeupFd_, &one, sizeof(one));
  if (n != sizeof(one)) {
    LOG_ERROR("EventLoop::handleRead() reads %d bytes instead of 8", n);
  }
}
