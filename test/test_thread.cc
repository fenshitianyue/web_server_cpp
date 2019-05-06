#include <iostream>
#include <thread>

void func(){
  std::cout << "hello thread1" << std::endl;
}

int main() {
  std::thread t(func); 
  t.join();
  return 0;
}
