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

void EpollPoller::removeChannel(Channel* channel) {
  // 从epoll中删除channel，更改状态，然后从Poller的channel_map删除这个channel
  int fd = channel->fd();
  channels_.erase(fd);

  LOG_INFO("func=%s => fd=%d\n", __FUNCTION__, fd);

  int index = channel->index();
  if (index == kAdded) {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(kNew);
}

// 填充活跃的连接
void EpollPoller::fillActiveChannels(int numEvents,
                                     ChannelList* activeChannels) const {
  for (int i = 0; i < numEvents; ++i) {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}

// 更新channel通道
void EpollPoller::update(int operation, Channel* channel) {
  // 根据operator:
  // EPOLL_CTL_ADD/MOD/DEL来具体调用epoll_ctl更改这个channel对应的fd在epoll上的监听事件
  epoll_event event;
  memset(&event, 0, sizeof(event));
  event.events = channel->events();
  event.data.ptr = channel;
  // epoll不关心里面的内容，用户可以通过ptr附带一些自定义消息
  int fd = channel->fd();
  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
    if (operation == EPOLL_CTL_DEL) {
      LOG_ERROR("epoll_ctl del error: %d\n", errno);
    } else {
      LOG_FATAL("epoll_ctl add/mod error: %d\n", errno);
    }
  }
}

TimeStamp EpollPoller::poll(int timeoutMs, ChannelList* activeChannels) {
  // FIXME: 原作者这里是channels.size()
  LOG_DEBUG("func=%s => fd total count: %lu\n", __FUNCTION__, channels_.size());
  int numEvents = epoll_wait(epollfd_, &*events_.begin(),
                             static_cast<int>(events_.size()), timeoutMs);
  int saveErrno = errno;
  TimeStamp now(TimeStamp::now());
  if (numEvents > 0) {
    LOG_DEBUG("%d events happened\n", numEvents);
    fillActiveChannels(numEvents, activeChannels);
    if (numEvents == events_.size()) {
      events_.resize(events_.size() * 2);
    }
  } else if (numEvents == 0) {
    LOG_DEBUG("%s timeout!\n", __FUNCTION__);
  } else {
    if (saveErrno != EINTR) {
      errno = saveErrno;
      LOG_ERROR("EPollPoller::poll() err!");
    }
  }
  return now;
}
