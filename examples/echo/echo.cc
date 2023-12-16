#include "echo.h"

#include <lzj_muduo/Buffer.h>
#include <lzj_muduo/Logger.h>
#include <lzj_muduo/TcpConnection.h>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

// using namespace muduo;
// using namespace muduo::net;

EchoServer::EchoServer(EventLoop* loop, const InetAddress& listenAddr)
    : server_(loop, listenAddr, "EchoServer") {
  server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
  server_.setMessageCallback(
      std::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

void EchoServer::start() { server_.start(); }

void EchoServer::onConnection(const TcpConnectionPtr& conn) {
  std::string tmp(conn->connected() ? "UP" : "DOWN");
  LOG_INFO("EchoServer - %s -> %s is %s",
           conn->peerAddress().toIpPort().c_str(),
           conn->localAddress().toIpPort().c_str(), tmp.c_str());
}

void EchoServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf,
                           TimeStamp time) {
  std::string msg(buf->retrieveAllString());
  LOG_INFO("%s echo %d bytes, data received at %s", conn->name().c_str(),
           msg.size(), time.toString().c_str());
  conn->send(msg);
}
