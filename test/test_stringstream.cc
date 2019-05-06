#include <iostream>
#include <sstream>
#include <string>


int main() {
  //C++ int -> string
  std::stringstream stream; 
  std::string result;
  int number = 12345;
  stream << number;
  stream >> result;
  std::cout << result << std::endl;
  //C int -> char*
  stream.clear();
  char result_c[6];
  stream << number;
  stream >> result_c;
  std::cout << result_c << std::endl;
  return 0;
}
