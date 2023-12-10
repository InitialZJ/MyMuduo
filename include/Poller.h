#ifndef POLLER_H_
#define POLLER_H_

#include <unordered_map>
#include <vector>

#include "Channel.h"
#include "noncopyable.h"

class Channel;
class EventLoop;
class Poller : public muduo::noncopyable {
 public:
  using ChannelList = std::vector<Channel*>;
  Poller(EventLoop* loop);
  virtual ~Poller() {}

  virtual TimeStamp poll(int timeoutMs, ChannelList* activateChannels) = 0;

  virtual void updateChannel(Channel* channel) = 0;
  virtual void removeChannel(Channel* channel) = 0;

  bool hasChannel(Channel* channel) const;

  static Poller* newDefaultPoller(EventLoop* loop);

 protected:
  using ChannelMap = std::unordered_map<int, Channel*>;
  ChannelMap channels_;

 private:
  EventLoop* ownerLoop_;
};

#endif  // !POLLER_H_
