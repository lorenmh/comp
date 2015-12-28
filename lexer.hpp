#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>

typedef enum Type {
  ID,
  FUNCTION,
  ARROW_FUNCTION,
  STRING,
  FLOAT,
  INT,
  LPAREN,
  RPAREN,
  LBRACK,
  RBRACK,
  LCURLY,
  RCURLY,
  QUOTE,
  APOSTROPHE,
  DASH,
  ASTERISK,
  PERCENT,
  PLUS,
  EQUALS,
  UNKNOWN,
  END_OF_FILE
} Type;

static std::map<std::string, Type> StringToType = {
  {"function", FUNCTION},
  {"=>", ARROW_FUNCTION},
  {"(", LPAREN},
  {")", RPAREN},
  {"[", LBRACK},
  {"]", RBRACK},
  {"{", LCURLY},
  {"}", RCURLY},
  {"\"", QUOTE},
  {"'", APOSTROPHE},
  {"-", DASH},
  {"*", ASTERISK},
  {"%", PERCENT},
  {"+", PLUS},
  {"=", EQUALS}
};

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
    if (this != &symbol_) {
      size = symbol_.size;
      ptr = (void*) malloc(size);
      memcpy(ptr, symbol_.ptr, size);
    }

    return *this;
  }

} Symbol;

static Symbol NULL_SYMBOL(NULL, 0);

typedef struct Token {
  Type type;
  Symbol symbol;

  Token(Type type_, Symbol symbol_)
  : symbol(symbol_)
  {
    type = type_;
  }  

  Token(Type type_)
  : symbol(NULL_SYMBOL)
  {
    type = type_;
  }

} Token;

static Token END_OF_FILE_TOKEN(END_OF_FILE, NULL_SYMBOL);

Token nextToken() {
  char currentChar = getchar();

  while(isspace(currentChar))
    currentChar = getchar();

  if (isalpha(currentChar)) {
    std::string id(&currentChar);
    while(isalnum(currentChar))
      id += currentChar;
  }

  return END_OF_FILE_TOKEN;
}

#endif
