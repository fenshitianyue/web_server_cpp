#include "server_base.hpp"
#include <fstream>

template<typename SERVER_TYPE>
void start_server(SERVER_TYPE& server){
  //向服务器添加请求资源的方法
  //1.处理默认 GET 请求，如果没有其他匹配成功，则这个函数将应答wwwroot/目录下的index.html文件
  server.default_resource["^/?(.*)$"]["GET"] = [](std::ostream& response, WebServer::Request& request){
    std::string filename = "wwwroot/";
    std::string path = request.path_match[1];
    //防止客户端使用'..'来访问wwwroot/目录之外的内容
    size_t last_pos = path.rfind(".");
    size_t current_pos = 0;
    size_t pos = 0;
  };

}
