#pragma once

#include <boost/asio.hpp>
#include <regex>
#include <unordered_map>
#include <thread>


namespace WebServer{
  //定义一个结构用于解析HTTP请求
  struct Request{
    //成员：请求方法：POST，GET；请求路径；HTTP版本
    std::string method, path, http_version;
    //对content使用智能指针管理
    std::shared_ptr<std::istream> content;
    std::unordered_map<std::string, std::string> header;
    //用正则表达式处理路径匹配
    std::smatch path_match;
  };
  //定义服务器资源类型
  typedef std::map<std::string, std::unordered_map<std::string, std::function<void(std::ostream&, Request&)>>> resource_type;

  //socket_type为HTTP/HTTPS
  template <typename socket_type>
  class ServerBase{
  public:
    ServerBase(unsigned short port, size_t num_threads = 1) : endpoint(boost::asio::ip::tcp::v4(), port),
                                                              acceptor(m_io_service, endpoint),
                                                              num_threads(num_threads){}
    //启动服务器
    void start(){
      //将资源添加到all_resource中(注：1.将默认资源添加到末尾作为默认应答方法 2.添加到all_resource中的是指向资源的迭代器)
      for(auto it = resource.begin(); it != resource.end(); ++it){
        all_resources.push_back(it);
      } 
      for(auto it = default_resource.begin(); it != resource.end(); ++it){
        all_resources.push_back(it);
      }
      //建立连接
      accept();
      //如果线程池中线程个数大于1，那么缺省运行(num_threads-1)线程
      //???
      for(size_t i = 1; i < num_threads; ++i){
        threads.emplace_back([this](){
              m_io_service.run(); 
        });
      }
      //运行主线程
      m_io_service.run();
      //等待其他线程
      for(auto& it : threads){
        it.join(); 
      }
    }
  protected:
    //需要不同类型的服务器实现这个方法
    virtual void accept(){};
    //处理请求和应答
    void process_request_and_respond(std::shared_ptr<socket_type> socket) const {
      
    }
  public:
    //定义服务器访问资源处理方式
    resource_type resource;
    //用于保存默认资源处理方式
    resource_type default_resource;
  protected:
    //用于内部实现对所有资源的处理
    //所有的资源/默认资源都会添加到这里，并在start()中创建
    std::vector<resource_type::iterator> all_resources;
  protected:
    //asio 库中的 io_service 是调度器，所有的异步IO事件都要通过它来分发处理
    //即，需要IO的对象的构造函数，都需要传入一个 io_service
    boost::asio::io_service m_io_service;
    //IP地址、端口号、协议版本构成一个endpoint，并通过这个endpoint在服务端生成
    //tcp::acceptor对象，并在指定端口上等待连接
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::acceptor acceptor;
  protected:
    //线程池
    size_t num_threads;
    std::vector<std::thread> threads;
  };

  template <typename socket_type>
  class Server : public ServerBase<socket_type> {};
}
