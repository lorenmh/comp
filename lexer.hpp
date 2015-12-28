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

typedef struct Symbol {
  void* ptr;
  int size;

  ~Symbol() {
    if (ptr)
      free(ptr);
  }

  Symbol(void* ptr_, int size_) {
    ptr = ptr_;
    size = size_;
  }

  Symbol(Symbol const& symbol_) {
    size = symbol_.size;
    ptr = (void*) malloc(size);
    memcpy(ptr, symbol_.ptr, size);
  }

  Symbol& operator= (Symbol const& symbol_) {
    std::cout << "ASSIGNMENT\n";

    if (this != &symbol_) {
      size = symbol_.size;
      ptr = (void*) malloc(size);
      memcpy(ptr, symbol_.ptr, size);
    }

    return *this;
  }

} Symbol;

typedef struct Token {
  Type type;
  Symbol symbol;
} Token;

#endif
