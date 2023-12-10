#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>
#include <memory>

#include "Timestamp.h"
#include "noncopyable.h"

class EventLoop;
/**
 * @brief
 * 封装了sockfd和它感兴趣的event，比如EPOLLIN和EPOLLOUT事件，绑定了poller返回的具体事件（回调函数）
 * 这里只实现了epoll没有实现poll
 */
class Channel : public muduo::noncopyable {
 public:
  using EventCallback = std::function<void()>;
  using ReadEventCallback = std::function<void(TimeStamp)>;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  void HandlerEvent(TimeStamp receive_time);

  void setReadCallback(ReadEventCallback cb) { read_callback_ = std::move(cb); }
  void setWriteCallback(EventCallback cb) { write_callback_ = std::move(cb); }
  void setCloseCallback(EventCallback cb) { close_callback_ = std::move(cb); }
  void setErrorCallback(EventCallback cb) { error_callback_ = std::move(cb); }
  void tie(const std::shared_ptr<void>&);
  int fd() const { return fd_; }
  int events() const { return events_; }

  int set_revents(int revt) { revents_ = revt; }

  void enableReading() {
    events_ |= kReadEvent;
    update();
  }
  void disableReading() {
    events_ &= ~kReadEvent;
    update();
  }
  void enableWriting() {
    events_ |= kWriteEvent;
    update();
  }
  void disableWriting() {
    events_ &= ~kWriteEvent;
    update();
  }
  void disableAll() {
    events_ = kNoneEvent;
    update();
  }
  bool isWriting() const { return events_ & kWriteEvent; }
  bool isReading() const { return events_ & kReadEvent; }
  bool isNoneEvent() const { return events_ == kNoneEvent; }

  int index() { return index_; }
  void set_index(int idx) { index_ = idx; }

  // one loop per thread
  EventLoop* ownerLoop() { return loop_; }
  void remove();

 private:
  void update();
  void HandleEventWithGuard(TimeStamp receiveTime);

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop* loop_;  // 属于哪个EvenLoop对象
  const int fd_;
  int events_;   // 感兴趣的事情
  int revents_;  // poller返回的具体发生的事情
  int index_;    // channel的状态

  std::weak_ptr<void> tie_;
  bool tied_;
  ReadEventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback close_callback_;
  EventCallback error_callback_;
};

#endif  // !CHANNEL_H_
