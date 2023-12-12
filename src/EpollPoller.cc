#include "EpollPoller.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "Channel.h"
#include "Logger.h"

// channel的三种状态
// channel未添加到poller中
const int kNew = -1;
// channel已添加到poller中
const int kAdded = 1;
// channel从poller中删除
const int kDeleted = 2;

EpollPoller::EpollPoller(EventLoop* loop)
    : Poller(loop),
      epollfd_(epoll_create1(EPOLL_CLOEXEC)),
      events_(kInitEventListSize) {
  if (epollfd_ < 0) {
    LOG_FATAL("epoll create error: %d\n", errno);
  }
}

EpollPoller::~EpollPoller() { close(epollfd_); }

void EpollPoller::updateChannel(Channel* channel) {
  // channel update/remove => EventLoop updateChannel/removeChannel => Poller
  // updateChannel/removeChannel
  const int index = channel->index();
  LOG_INFO("fd=%d events=%d index=%d\n", channel->fd(), channel->events(),
           index);
  if (index == kNew || index == kDeleted) {
    if (index == kNew) {
      // 这个channel从未添加到poller中，那么添加到poller的channel_map中
      int fd = channel->fd();
      channels_[fd] = channel;
    }
    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  } else {
    // channel已经在poller上注册过了
    int fd = channel->fd();
    if (channel->isNoneEvent()) {
      // 对任何事情都不感兴趣
      update(EPOLL_CTL_DEL, channel);
			channel->set_index(kDeleted);
    } else {
			// 还是对某些事情感兴趣，修改已经注册的fd的监听事件
			update(EPOLL_CTL_MOD, channel);
		}
  }
}
