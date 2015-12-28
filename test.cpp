
#include "lexer.hpp"
#include <iostream>
#include <cstring>

Token createToken() {
  char* str = (char*) malloc(6 * sizeof(char));
  strncpy(str, "eyyyy", 6);
  Symbol sym(str, 6 * sizeof(char));

  Token token = { ID, sym };

  return token;
}

void printToken(Token const& token) {
  std::cout << "TOKEN: " << &token << '\n';
  std::cout << "TYPE: " << token.type << '\n';
  std::cout << "SYMBOL: " << &token.symbol << '\n';
  std::cout << "SYMBOL_STR: " << (char*) token.symbol.ptr << '\n';
  std::cout << "SYMBOL_SIZE: " << token.symbol.size << "\n\n";

}

int main() {
  Token foo = createToken();
  printToken(foo);
  Token bar(foo);
  bar = foo;
  printToken(bar);
}
