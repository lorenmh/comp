
#include "lexer.hpp"
#include <iostream>
#include <cstring>

Token foo() {
  char* str = (char*) malloc(6 * sizeof(char));
  strncpy(str, "eyyyy", 6);

  Token token = {
    ID,
    str
  };

  return token;
}

int main() {
  Token bar = foo();
  std::cout << "FROM BAR: " << bar.type << ", " << (char*)bar.symbol << '\n';
}
