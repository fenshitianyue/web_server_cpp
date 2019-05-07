#include <iostream>
#include <boost/asio.hpp> //Boost Asio模块
#include <boost/asio/ssl.hpp> //Asio模块如果要建立HTTPS连接，就要依赖OpenSSL框架

//Asio需要建立一个和socket有关的对象，即 boost::asio::ip::tcp::socket，这里将其简化重命名为HTTP
typedef boost::asio::ip::tcp::socket HTTP;

void test_http(){
  ///////////////////////////////////////////////////////
  //对于使用Asio的程序，必须包含一个io_service对象。
  //Asio这个模块抽象了诸如网络、串口通信等概念，并将其统一归为IO操作
  //所以io_service这个类提供了访问I/O的功能
  ///////////////////////////////////////////////////////
  boost::asio::io_service io;
  HTTP socket(io); //HTTP对象必须用io_service的对象进行构造
  unsigned short port = 8080;
  ///////////////////////////////////////////////////////
  //由于socket是一个端对端的连接，所以Asio要定义一个endpoint类型对象作为
  //socket服务端的一个端点
  //endpoint构造参数：ip地址和端口号
  ///////////////////////////////////////////////////////
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
  ///////////////////////////////////////////////////////
  // Asio要建立连接，就要定义一个acceptor类型的对象
  // acceptor构造参数：io_service类型对象和endpoint类型对象
  ///////////////////////////////////////////////////////
  boost::asio::ip::tcp::acceptor acceptor(io, endpoint);

  //IO操作
  //定义一个流缓冲区
  boost::asio::streambuf read_buf;
  ///////////////////////////////////////////////////////
  // 很多网络协议都是基于行实现的，也就是说这些协议元素由\r\n
  // 符号进行界定，eg:HTTP。在Asio中，读取分隔符的协议的函数是：
  // async_read_untile()
  ///////////////////////////////////////////////////////
  boost::asio::async_read_until(socket, read_buf, "\r\n\r\n", read_handler); 
}

void test_https(){
  
}

int main() {

  return 0;
}
