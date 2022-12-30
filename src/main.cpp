#include <iostream>

int main(int argc, char **argv) {
  std::cout << "======================================" << std::endl;
  std::cout << "| OptiCraft Asset Pack Compiler v0.0 |" << std::endl;
  std::cout << "======================================" << std::endl
            << std::endl;

  std::cout << "Arguments: ";

  for (int i = 1; i < argc; i++) {
    std::cout << argv[i];
    if (i != argc - 1) std::cout << ", ";
  }
  std::cout << std::endl;

  return 0;
}