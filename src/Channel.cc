#include "Channel.h"

#include <EventLoop.h>
#include <sys/epoll.h>

#include "Logger.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), tied_(false) {}

Channel::~Channel() {}

void Channel::tie(const std::shared_ptr<void>& obj) {
  tie_ = obj;
  tied_ = true;
}

void Channel::update() { loop_->updateChannel(this); }

void Channel::remove() { loop_->removeChannel(this); }

void Channel::HandlerEvent(TimeStamp receiveTime) {
  if (tied_) {
    std::shared_ptr<void> guard = tie_.lock();
    if (guard) {
      HandleEventWithGuard(receiveTime);
    }
  } else {
    HandleEventWithGuard(receiveTime);
  }
}

void Channel::HandleEventWithGuard(TimeStamp receiveTime) {
  LOG_INFO("channel Handle Event revents: %d", revents_);
  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
    // 连接断开
    if (close_callback_) {
      close_callback_();
    }
  }
  if (revents_ & EPOLLERR) {
    if (error_callback_) {
      error_callback_();
    }
  }
  if (revents_ & (EPOLLIN | EPOLLPRI)) {
    if (read_callback_) {
      read_callback_(receiveTime);
    }
  }
  if (revents_ & EPOLLOUT) {
    if (write_callback_) {
      write_callback_();
    }
  }
}
