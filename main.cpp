#include <iostream>
#include "include/erupt.h"
int main() {

  std::cout << "Erupt example :" << std::endl;
    EInstance instance = EInstance(false,false,0,0,"Example");
  std::cout << "End of example" << std::endl;
  return EXIT_SUCCESS;
};