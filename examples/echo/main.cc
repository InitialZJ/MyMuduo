#include <lzj_muduo/EventLoop.h>
#include <lzj_muduo/Logger.h>
#include <unistd.h>

#include "echo.h"

// using namespace muduo;
// using namespace muduo::net;

int main() {
  LOG_INFO("pid = %d\n", static_cast<int>(getpid()));
  EventLoop loop;
  InetAddress listenAddr(2009);
  EchoServer server(&loop, listenAddr);
  server.start();
  loop.loop();
}
