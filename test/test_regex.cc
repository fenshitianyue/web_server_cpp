#include <iostream>
#include <string>
#include <regex>

//这是一个测试regex的文件

int main() {
  std::string fnames[] = {"foo.txt", "bar.txt", "test", "a0.txt", "AAA.txt"};
  //在C++中 `\` 会被作为字符串内的转义符，为使 `\.` 作为正则表达式传递进去生效，需要对 `\` 进行二次转义，从而有 `\\.`
  std::regex txt_regex("[a-z]+\\.txt");
  for(const auto& it : fnames)
    std::cout << it << ": " << std::regex_match(it, txt_regex) << std::endl;
  return 0;
}
