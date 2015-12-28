#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string.h>

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

static std::map<std::string, Type> stringToType = {
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

bool isLastCharSet = false;
char lastChar;

Token nextToken() {
  char currentChar;

  if (isLastCharSet) {
    currentChar = lastChar;
  } else {
    currentChar = getchar();
  }
    
  while (isspace(currentChar))
    currentChar = getchar();

  if (isalpha(currentChar)) {
    std::string alphanumeric(&currentChar);

    while (isalnum(currentChar = getchar())) {
      alphanumeric += currentChar;
    }

    lastChar = currentChar;
    isLastCharSet = true;

    auto search = stringToType.find(alphanumeric);
    if (search == stringToType.end()) {
      std::string* symbolStr = new std::string(alphanumeric);
      return Token(ID, Symbol(symbolStr, symbolStr->size()));
    } else {
      return Token((Type) search->second);
    }
  } else if (isdigit(currentChar) || currentChar == '.') {
    std::string numStr;

    bool hasDecimal = false;

    do {
      if (currentChar == '.') {
        hasDecimal = true;
      }
      numStr += currentChar;
      currentChar = getchar();
    } while (isdigit(currentChar) || currentChar == '.');

    lastChar = currentChar;
    isLastCharSet = true;

    if (hasDecimal) {
      float* symbolFloat = new float(strtof(numStr.c_str(), 0));
      return Token(FLOAT, Symbol(symbolFloat, sizeof(float)));
    } else {
      int* symbolInt = new int(atoi(numStr.c_str()));
      return Token(INT, Symbol(symbolInt, sizeof(int)));
    }
  } else if (currentChar != EOF) {
    std::string tokenString(&currentChar);

    while (!isspace(currentChar = getchar()))
      tokenString += currentChar;

    lastChar = currentChar;
    isLastCharSet = true;

    auto search = stringToType.find(tokenString);
    if (search == stringToType.end()) {
      return Token(UNKNOWN);
    } else {
      return Token((Type) search->second);
    }
  }
  std::cout << "here!??!?\n";
  return EOF_TOKEN;
}
