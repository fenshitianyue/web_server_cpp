#pragma once

#include <boost/asio.hpp>
#include <regex>
#include <unordered_map>
#include <thread>


namespace WebServer{
  //定义一个结构用于解析HTTP请求
  struct Request{
    std::string method, path, http_version;
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
      //为 async_read_untile() 创建新的读缓存 
      //shared_ptr 用于传递临时对象给匿名函数
      auto read_buffer = std::make_shared<boost::asio::streambuf>(); //auto : std::shared_ptr<boost::asio::streambuf>
      boost::asio::async_read_until(*socket, *read_buffer, "\r\n\r\n",
          [this, socket, read_buffer](const boost::system::error_code& ec, size_t bytes_transferred){
            if(!ec){
              size_t total = read_buffer->size();
               
              std::istream stream(read_buffer.get());
              auto request = std::make_shared<Request>();
              *request = parse_request(stream);

              size_t num_additional_bytes = total - bytes_transferred;
              if(request->header.count("Content-Length") > 0){
                boost::asio::async_read(*socket, *read_buffer, 
                                        boost::asio::transfer_exactly(stoull(request->header["Content-Length"]) - num_additional_bytes), 
                                        [this, socket, read_buffer, request](const boost::system::error_code& ec, size_t){
                                          if(!ec){
                                            request->content = std::shared_ptr<std::istream>(new std::istream(read_buffer.get()));
                                            respond(socket, request);
                                          }
                                        });
                }else{
                  respond(socket, request);
                }
            }

          });
    }
    //解析请求
    Request parse_request(std::istream& stream) const {
      Request request;
      //通过正则表达式解析出请求方法、请求路径和HTTP版本号
      std::regex r("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
      std::smatch sub_match;
      //从第一行中解析出请求方法、请求路径和HTTP版本号
      std::string line;
      getline(stream, line);
      //line.pop_back(); 
      if(std::regex_match(line, sub_match, r)){
        request.method = sub_match[1];
        request.path = sub_match[2];
        request.http_version = sub_match[3];

        bool matched;
        r = "^([^:]*): ?(.*)$";
        //解析头部的其他信息
        do{
          std::string().swap(line); //释放line底层空间
          getline(stream, line); 
          //line.pop_back();
          matched = std::regex_match(line, sub_match, r);
          if(matched){
            request.header[sub_match[1]] = sub_match[2];
          }
        }while(matched == true);
      }
      return request;
    }
    void respond(std::shared_ptr<socket_type> socket, std::shared_ptr<Request> request) const {
      //对请求路径和方法进行匹配查找，并生成响应
      for(auto res_it : all_resources){
        std::regex e(res_it->first);
        std::smatch sm_res;
        if(std::regex_match(request->path, sm_res, e)){
          if(res_it->second.count(request->method) > 0){
            request->path_match = move(sm_res);
            auto write_buffer = std::make_shared<boost::asio::streambuf>();
            std::ostream response(write_buffer.get());
            res_it->second[request->method](response, *request);

            //由于是异步写，所以要在lambda中捕获write_buffer，确保其不会在异步写完成前销毁
            boost::asio::async_write(*socket, *write_buffer, 
                [this, socket, request, write_buffer](const boost::system::error_code ec, size_t){
                  if(!ec && stof(request->http_version) > 1.05)
                    process_request_and_respond(socket);
                });
            return; //???
          }
        }
      }
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
