
#include "lexer.hpp"
#include <iostream>
#include <cstring>

Token createToken() {
  char* str = (char*) malloc(6 * sizeof(char));
  strncpy(str, "eyyyy", 6);

  Token token(ID, str);

  return token;
}

void printToken(Token const& token) {
  std::cout << "TOKEN: " << &token << '\n';
  std::cout << "TYPE: " << token.type << '\n';
  std::cout << "SYMBOL: " << token.symbol << '\n';
  std::cout << "SYMBOL_STR: " << (char*) token.symbol << "\n\n";
}

int main() {
  Token foo = createToken();
  printToken(foo);
  Token bar(foo);
  printToken(bar);
}
