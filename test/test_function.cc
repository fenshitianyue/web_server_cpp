#include <iostream>
#include <functional>

int func(int n){
  return n;
}

int main() {
  std::function<int(int)> f = func; 
  std::cout << f(10) << std::endl;
  return 0;
}
