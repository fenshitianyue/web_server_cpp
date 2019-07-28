#include <iostream>
#include "server_http.hpp"
#include "handler.hpp"


int main() {
  //HTTP服务器运行端口号：8080 | 线程池初始化为4个线程
  WebServer::Server<WebServer::HTTP> server(8080, 4);
  std::cout << "Server run." << std::endl;
  start_server<WebServer::Server<WebServer::HTTP>>(server); 
  return 0;
}
