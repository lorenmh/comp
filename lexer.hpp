#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string.h>
#include <fstream>
#include "lexertl/generator.hpp"
#include "lexertl/lookup.hpp"
#include "lexertl/match_results.hpp"
#include "lexertl/stream_shared_iterator.hpp"

typedef enum Type {
  FUNCTION = 1,
  ARROW_FUNCTION,
  RETURN,
  VAR,
  STRING_LITERAL,
  FLOAT,
  INT,
  LPAREN,
  RPAREN,
  LBRACK,
  RBRACK,
  LCURLY,
  RCURLY,
  COMMA,
  DASH,
  ASTERISK,
  FSLASH,
  PERCENT,
  PLUS,
  EQUALS,
  DOUBLE_EQUALS,
  COLON,
  SEMICOLON,
  ID,
  UNKNOWN,
} Type;

static std::map<Type, std::string> typeToRegex = {
  {FUNCTION, "function"},
  {ARROW_FUNCTION, "\\=\\>"},
  {RETURN, "return"},
  {VAR, "var"},
  {STRING_LITERAL, "[\"](\\\\.|[^\"])*[\"]|['](\\\\.|[^'])*[']"},
  {FLOAT, "\\d+\\.\\d*|\\d*\\.\\d+"},
  {INT, "\\d+"},
  {LPAREN, "\\("},
  {RPAREN, "\\)"},
  {LBRACK, "\\["},
  {RBRACK, "\\]"},
  {LCURLY, "\\{"},
  {RCURLY, "\\}"},
  {COMMA, ","},
  {DASH, "\\-"},
  {ASTERISK, "\\*"},
  {FSLASH, "\\/"},
  {PERCENT, "\\%"},
  {PLUS, "\\+"},
  {EQUALS, "\\="},
  {DOUBLE_EQUALS, "\\=\\="},
  {COLON, "\\:"},
  {SEMICOLON, "\\;"},
  {ID, "[a-zA-Z]\\w*"},
};

static std::map<Type, std::string> typeToTitle = {
  {FUNCTION, "FUNCTION"},
  {ARROW_FUNCTION, "ARROW_FUNCTION"},
  {RETURN, "RETURN"},
  {VAR, "VAR"},
  {STRING_LITERAL, "STRING_LITERAL"},
  {FLOAT, "FLOAT"},
  {INT, "INT"},
  {LPAREN, "LPAREN"},
  {RPAREN, "RPAREN"},
  {LBRACK, "LBRACK"},
  {RBRACK, "RBRACK"},
  {LCURLY, "LCURLY"},
  {RCURLY, "RCURLY"},
  {COMMA, "COMMA"},
  {DASH, "DASH"},
  {ASTERISK, "ASTERISK"},
  {FSLASH, "FSLASH"},
  {PERCENT, "PERCENT"},
  {PLUS, "PLUS"},
  {EQUALS, "EQUALS"},
  {DOUBLE_EQUALS, "DOUBLE_EQUALS"},
  {COLON, "COLON"},
  {SEMICOLON, "SEMICOLON"},
  {ID, "ID"},
  {UNKNOWN, "UNKNOWN"}
};

typedef struct Attribute {
  void* ptr;
  int size;

  ~Attribute() {
    if (ptr)
      free(ptr);
  }

  Attribute(void* ptr_, int size_) {
    ptr = ptr_;
    size = size_;
  }

  Attribute(Attribute const& attribute_) {
    size = attribute_.size;
    if (size) {
      ptr = (void*) malloc(size);
      memcpy(ptr, attribute_.ptr, size);
    } else {
      ptr = NULL;
    }
  }

  Attribute& operator= (Attribute const& attribute_) {
    if (this != &attribute_) {
      size = attribute_.size;
      if (size) {
        memcpy(ptr, attribute_.ptr, size);
      } else {
        ptr = NULL;
      }
    }

    return *this;
  }

} Attribute;

static Attribute NULL_ATTRIBUTE(NULL, 0);

typedef struct Token {
  Type type;
  Attribute attribute;

  Token(Type type_, Attribute attribute_)
  : attribute(attribute_)
  {
    type = type_;
  }  

  Token(Type type_)
  : attribute(NULL_ATTRIBUTE)
  {
    type = type_;
  }

} Token;

void printToken(Token const& token) {
  auto search = typeToTitle.find(token.type);
  
  if (search == typeToTitle.end()) {
    std::cout << '<' << token.type;
  } else {
    std::cout << '<' << search->second;
  }

  if (token.type == ID || token.type == STRING_LITERAL) {
    std::cout << ", " << *((std::string*)token.attribute.ptr) << ">\n";
  } else if (token.type == FLOAT) {
    std::cout << ", " << *((float*)token.attribute.ptr) << ">\n";
  } else if (token.type == INT) {
    std::cout << ", " << *((int*)token.attribute.ptr) << ">\n";
  } else {
    std::cout << ">\n";
  }
}

// Right now this will do everything from an in-memory representation of
// the tokens.  Best way would be to stream the tokens once they are made.
void tokensFromStdin() {
  lexertl::rules rules;
  lexertl::state_machine sm;

  for (auto& typeRegex : typeToRegex) {
    rules.push(typeRegex.second, typeRegex.first);
  }

  rules.push("*", "[ \t\r\n]+", rules.skip(), ".");

  lexertl::generator::build(rules, sm);

  std::ifstream ifile("/dev/stdin");
  lexertl::stream_shared_iterator iter(ifile);
  lexertl::stream_shared_iterator end;
  lexertl::match_results<lexertl::stream_shared_iterator> result(iter, end);

  //lexertl::smatch result(input.begin(), input.end());

  // TODO: do this as an iterator
  std::vector<Token> tokens;
  for (lexertl::lookup(sm, result);
       result.id != 0;
       lexertl::lookup(sm, result))
  {
    if (result.id == ID || result.id == STRING_LITERAL) {

      std::string* str = new std::string(result.str());
      Attribute attr(str, str->size() + 1);
      tokens.push_back(Token((Type)result.id, attr));

    } else if (result.id == INT) {

      int* intVal = new int(atoi(result.str().c_str()));
      Attribute attr(intVal, sizeof(int));
      tokens.push_back(Token((Type)result.id, attr));

    } else if (result.id == FLOAT) {

      float* floatVal = new float(std::stof(result.str()));
      Attribute attr(floatVal, sizeof(float));
      tokens.push_back(Token((Type)result.id, attr));

    } else {
      tokens.push_back(Token((Type)result.id));
    }
  }

  for (auto& token : tokens) {
    printToken(token);
  }
}
