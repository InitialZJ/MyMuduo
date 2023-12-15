#ifndef SOCKET_H_
#define SOCKET_H_

#include "noncopyable.h"
class InetAddress;
class Socket : public muduo::noncopyable {
 public:
  explicit Socket(int sockfd) : sockfd_(sockfd) {}
  ~Socket();

 public:
  int fd() const { return sockfd_; }
  void bindAddress(const InetAddress& localaddr);
  void listen();
  int accept(InetAddress* peeradd);
  void shutdownWrite();

  void setTcpNoDelay(bool on);
  void setReuseAddr(bool on);
  void setReusePort(bool on);
  void setKeepAlive(bool on);

 private:
  const int sockfd_;
};

#endif  // !SOCKET_H_
