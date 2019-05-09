#include <iostream>
#include <string>


void test_clear(){
  std::string line; 
  std::cin >> line;
  std::cout << "before:" << std::endl;
  std::cout << line << std::endl;
  std::cout << "size:" << line.size() << std::endl;
  std::cout << "capacity:" << line.capacity() << std::endl;
  std::cout << "clear after:" << std::endl;
  line.clear();
  std::cout << "size:" << line.size() << std::endl;
  std::cout << "capacity:" << line.capacity() << std::endl;
}

void test_swap(){
  std::string line; 
  std::cin >> line;
  std::cout << "before:" << std::endl;
  std::cout << line << std::endl;
  std::cout << "size:" << line.size() << std::endl;
  std::cout << "capacity:" << line.capacity() << std::endl;
  std::cout << "clear after:" << std::endl;
  std::string().swap(line);
  std::cout << "size:" << line.size() << std::endl;
  std::cout << "capacity:" << line.capacity() << std::endl;
  line = "world";
  std::cout << line << std::endl;
  std::cout << "size:" << line.size() << std::endl;
  std::cout << "capacity:" << line.capacity() << std::endl;
}

int main() {
  //test_clear();
  test_swap();
  return 0;
}
