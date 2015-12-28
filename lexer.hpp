#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef enum Type {
  ID,
  FUNCTION,
  STRING,
  FLOAT,
  INT,
  LPAREN,
  RPAREN,
  LBRACK,
  RBRACK,
  DASH,
  ASTERISK,
  PERCENT,
  PLUS,
  EQUALS
} Type;

typedef struct Token {
  Type type;
  void* symbol;
  ~Token() {
    std::cout << "DESTRUCTING!\nSYMBOL ADDRESS: " << symbol << '\n';
    if (symbol)
      free(symbol);
  }
} Token;

#endif
