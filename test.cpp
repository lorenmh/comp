
#include <iostream>
#include <cstring>
#include <vector>

#include "lexer.hpp"

int main() {
  std::vector<Token> tokens;

  Token currentToken = nextToken();
  while (currentToken.type != END_OF_FILE) {
    printToken(currentToken);
    currentToken = nextToken();
  }

}
