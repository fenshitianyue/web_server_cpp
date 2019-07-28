#include "server_base.hpp"
#include <fstream>

template<typename SERVER_TYPE>
void start_server(SERVER_TYPE& server) {
  //向服务器添加请求资源的方法
  //1.处理默认 GET 请求，如果没有其他匹配成功，则这个函数将应答wwwroot/目录下的index.html文件
  server.default_resource["^/?(.*)$"]["GET"] = [](std::ostream& response, WebServer::Request& request) {
    std::string filename = "wwwroot/";
    std::string path = request.path_match[1];
    //防止客户端使用'..'来访问wwwroot/目录之外的内容
    size_t last_pos = path.rfind(".");
    size_t current_pos = 0;
    size_t pos = 0;
    while((pos = path.find('.', current_pos)) != std::string::npos && pos != last_pos) {
      current_pos = pos;
      path.erase(pos, 1);
      --last_pos;
    }
    filename += path;
    std::ifstream ifs;
    //平台无关的文件或目录检查
    if(filename.find('.') == std::string::npos) {
      if(filename[filename.length() - 1] != '/')
        filename += '/';
      filename += "index.html";
    }
    ifs.open(filename, std::ifstream::in);
    if(ifs.is_open()) {
      ifs.seekg(0, ifs.end);  
      size_t length = ifs.tellg();
      ifs.seekg(0, ifs.beg);
      //文件内容拷贝到 response-stream 中，不适用于文件内容过大
      response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n" << ifs.rdbuf();
    } else {
      //文件不存在时，返回无法打开文件
      std::string content = "Could not open file" + filename;
      response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.size() << "\r\n\r\n" << content;
    }
  };
  
  //运行服务器
  server.start();
}
