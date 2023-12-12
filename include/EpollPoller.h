#ifndef EPOLL_POLLER_H_
#define EPOLL_POLLER_H_

#include <sys/epoll.h>

#include <vector>

#include "Poller.h"
#include "Timestamp.h"

class EpollPoller : public Poller {
 public:
  EpollPoller(EventLoop* loop);
  // 子类析构加override，如果父类析构没加virtual就会报错
  ~EpollPoller() override;

  TimeStamp poll(int timeoutMs, ChannelList* activeChannels) override;

  void updateChannel(Channel* channel) override;
  void removeChannel(Channel* channel) override;

 private:
  static const int kInitEventListSize = 16;

  // 填充活跃的连接
  void fillActiveChannels(int numEvents, ChannelList* activateChannels) const;

  // 更新channel通道
  void update(int operation, Channel* channel);

  using EventList = std::vector<epoll_event>;
  int epollfd_;  // 事件循环本身需要一个fd
  EventList events_;
};

#endif  // !EPOLL_POLLER_H_
