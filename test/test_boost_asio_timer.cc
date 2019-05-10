#include <iostream>
#include <boost/asio.hpp>
#include <thread>

void handler_1(const boost::system::error_code& ec){
  std::cout << "5 s." << std::endl;
}

void handler_2(const boost::system::error_code& ec){
  std::cout << "5 s." << std::endl;
}

boost::asio::io_service io_service;


int main() {
  boost::asio::deadline_timer timer_1(io_service, 
                                      boost::posix_time::seconds(5)); 
  timer_1.async_wait(handler_1);
  boost::asio::deadline_timer timer_2(io_service, 
                                      boost::posix_time::seconds(5)); 
  timer_2.async_wait(handler_2);
  std::thread thread_1( [](){io_service.run();} );
  std::thread thread_2( [](){io_service.run();} );
  thread_1.join(); 
  thread_2.join(); 
  return 0;
}
