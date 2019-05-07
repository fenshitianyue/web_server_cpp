#include "server_base.hpp"
#include <string>
#include <sstream>
#include <memory>

namespace WebServer{
  //定义一个结构用于解析HTTP请求
  struct Request{
    //成员：请求方法：POST，GET；请求路径；HTTP版本
    std::string method, path, http_version;
    //对content使用智能指针管理
    std::shared_ptr<std::istream> content;
    std::unordered_map<std::string, std::string> header;
    //用正则表达式处理路径匹配
    std::smatch path_math;
  };
  //定义服务器资源类型
  typedef std::map<std::string, std::unordered_map<std::string, std::function<void(std::ostream&, Request&)>>> resouce_type;

}
