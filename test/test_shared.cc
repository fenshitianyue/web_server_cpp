#include <iostream>
#include <memory>

//这是一个测试shared_ptr的文件
//编译命令：g++ test_shared.cc -std=c++11

void func(std::shared_ptr<int> n){
  (*n)++;
}

int main() {
  auto pointer = std::make_shared<int>(10); 
  func(pointer);
  std::cout << *pointer << std::endl;
  return 0;
}
