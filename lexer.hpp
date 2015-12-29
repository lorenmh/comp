#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string.h>
#include "lexertl/generator.hpp"
#include "lexertl/lookup.hpp"
#include "lexertl/match_results.hpp"

typedef enum Type {
  ID = 1,
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

static std::map<Type, std::string> typeToRegex = {
  {ID, "[a-zA-Z]\\w*"},
  {FUNCTION, "function"},
  {ARROW_FUNCTION, "\\=\\>"},
  {STRING, "[\"](.)*[\"]"}
};

static std::map<Type, std::string> typeToTitle = {
  {ID, "ID"},
  {FUNCTION, "FUNCTION"},
  {ARROW_FUNCTION, "ARROW_FUNCTION"},
  {STRING, "STRING"},
  {FLOAT, "FLOAT"},
  {INT, "INT"},
  {LPAREN, "LPAREN"},
  {RPAREN, "RPAREN"},
  {LBRACK, "LBRACK"},
  {RBRACK, "RBRACK"},
  {LCURLY, "LCURLY"},
  {RCURLY, "RCURLY"},
  {QUOTE, "QUOTE"},
  {APOSTROPHE, "APOSTROPHE"},
  {DASH, "DASH"},
  {ASTERISK, "ASTERISK"},
  {PERCENT, "PERCENT"},
  {PLUS, "PLUS"},
  {EQUALS, "EQUALS"},
  {UNKNOWN, "UNKNOWN"},
  {END_OF_FILE, "END_OF_FILE"}
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
    if (size) {
      ptr = (void*) malloc(size);
      memcpy(ptr, symbol_.ptr, size);
    } else {
      ptr = NULL;
    }
  }

  Symbol& operator= (Symbol const& symbol_) {
    if (this != &symbol_) {
      size = symbol_.size;
      if (size) {
        memcpy(ptr, symbol_.ptr, size);
      } else {
        ptr = NULL;
      }
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

static Token EOF_TOKEN(END_OF_FILE, NULL_SYMBOL);

void printToken(Token const& token) {
  auto search = typeToTitle.find(token.type);
  
  if (search == typeToTitle.end()) {
    std::cout << '<' << token.type;
  } else {
    std::cout << '<' << search->second;
  }

  if (token.type == ID) {
    std::cout << ", " << *((std::string*)token.symbol.ptr) << ">\n";
  } else if (token.type == FLOAT) {
    std::cout << ", " << *((float*)token.symbol.ptr) << ">\n";
  } else if (token.type == INT) {
    std::cout << ", " << *((int*)token.symbol.ptr) << ">\n";
  } else {
    std::cout << ">\n";
  }
}

void tokensFromLine() {
  lexertl::rules rules;
  lexertl::state_machine sm;

  for (auto& typeRegex : typeToRegex) {
    rules.push(typeRegex.second, typeRegex.first);
  }

  //rules.push("[0-9]+", 1);
  //rules.push("[a-z]+", 2);
  lexertl::generator::build(rules, sm);

  std::string input("abc012Ad3e4 function \"this is a string with a function text in it!\" blah => bar");
  lexertl::smatch results(input.begin(), input.end());

  // Read ahead
  lexertl::lookup(sm, results);
  
  while (results.id != 0)
  {
      std::cout << "Id: " << results.id << ", Token: '" <<
          results.str () << "'\n";
      lexertl::lookup(sm, results);
  }
}
