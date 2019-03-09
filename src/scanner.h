#ifndef calc_scanner_h
#define calc_scanner_h

#include "common.h"

typedef enum {
  TOKEN_NUMBER,

  TOKEN_MINUS,
  TOKEN_PLUS,
  TOKEN_SLASH,
  TOKEN_STAR,

  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,

  TOKEN_EOF,
  TOKEN_ERROR
} TokenType;

typedef struct {
  TokenType type;
  const char* start; 
  uint32_t length;
} Token;

typedef struct {
  const char* start;
  const char* current;
} Scanner;

void initScanner(Scanner* scanner, const char* source);
Token scanToken(Scanner* scanner);

#endif
