#pragma once

#include <boost/asio.hpp>
#include <regex>
#include <unordered_map>
#include <thread>


namespace WebServer{
  //socket_type为HTTP/HTTPS
  template <typename socket_type>
  class ServerBase{
  public:
    //启动服务器
    void start();
  protected:
    //需要不同类型的服务器实现这个方法
    virtual void accept(){};
    //处理请求和应答
    void process_request_and_respond(std::shared_ptr<socket_type> socket) const;
  };

  template <typename socket_type>
  class Server : public ServerBase<socket_type> {};
}
