#ifndef LLVMLEXER_H
#define LLVMLEXER_H

#include <string>
#include <stdio.h>
#include <cstdlib>

enum Token {
  tok_eof = -1,
  tok_def = -2,
  tok_extern = -3,
  tok_identifier = -4,
  tok_number = -5
};

std::string identifierStr;
double numVal;

int getToken() {
  static int currentChar = ' ';

  while (isspace(currentChar))
    currentChar = getchar();

  if (isalpha(currentChar)) {
    while (isalnum((currentChar = getchar())))
      identifierStr += currentChar;

    if (identifierStr == "def")
      return tok_def;

    if (identifierStr == "extern")
      return tok_extern;

    return tok_identifier;
  }

  if (isdigit(currentChar) || currentChar == '.') {
    std::string numStr;

    do {
      numStr += currentChar;
      currentChar = getchar();
    } while (isdigit(currentChar) || currentChar == '.');

    numVal = strtod(numStr.c_str(), 0);

    return tok_number;
  }

  if (currentChar == '#') {
    do
      currentChar = getchar();
    while (currentChar != EOF && currentChar != '\n' && currentChar != '\r');

    if (currentChar != EOF)
      return getToken();
  }

  if (currentChar == EOF)
    return tok_eof;

  int thisChar = currentChar;
  currentChar = getchar();
  return thisChar;
}

#endif
