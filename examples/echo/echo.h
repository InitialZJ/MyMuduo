#ifndef MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H
#define MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H

#include <lzj_muduo/TcpServer.h>

// RFC 862
class EchoServer {
 public:
  EchoServer(EventLoop* loop, const InetAddress& listenAddr);

  void start();  // calls server_.start();

 private:
  void onConnection(const TcpConnectionPtr& conn);

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp time);

  TcpServer server_;
};

#endif  // MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H
