#include <iostream>
#include <memory>
#include <array>
#include <boost/asio.hpp>


class session : public std::enable_shared_from_this<session> {
public:
  session(boost::asio::ip::tcp::socket s) : socket_(std::move(s)){}
  void start(){
    async_read();
  }
private:
  void async_read(){
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_), 
                              [this, self](const boost::system::error_code& ec, size_t bytes_transferred){
                                if(!ec){
                                  async_write(bytes_transferred);
                                }
                              }
                            );
  }
  
  void async_write(size_t length){
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                               [this, self](const boost::system::error_code& ec, size_t){
                                 if(!ec){
                                   async_read();
                                 }
                               }
                            );
  }
  boost::asio::ip::tcp::socket socket_;
  std::array<char, 1024> data_;
};

class server{
public:
  server(boost::asio::io_service& io_server, short port)
    : acceptor_(io_server, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), socket_(io_server){
      async_accept();
    }
private:
  void async_accept(){
    acceptor_.async_accept(socket_, std::bind(&server::handler_accept, this, std::placeholders::_1));
  }
  void handler_accept(const boost::system::error_code& ec){
    if(!ec){
      std::shared_ptr<session> session_ptr(new session(std::move(socket_)));
      session_ptr->start();
    }
    async_accept();
  }
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
};

int main() {

  boost::asio::io_service io_server; 
  server s(io_server, 8080);
  io_server.run();

  return 0;
}
