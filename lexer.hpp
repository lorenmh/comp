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
    if (symbol)
      free(symbol);
  }

  Token(Type type_, void* symbol_) {
    type = type_;
    symbol = symbol_;
  }

  Token(Token const& token) {
    type = token.type;
    symbol = (void*) malloc(sizeof(token.symbol));
    memcpy(symbol, token.symbol, sizeof(symbol));
  }

  Token& operator= (Token const& token) {
    std::cout << "ASSIGNMENT\n";

    if (this != &token) {
      type = token.type;
      symbol = (void*) malloc(sizeof(token.symbol));
      memcpy(symbol, token.symbol, sizeof(symbol));
    }

    return *this;
  }

} Token;

#endif
