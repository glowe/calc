#include <string.h>

#include "scanner.h"

void initScanner(Scanner *scanner, const char *source) {
  scanner->start = source;
  scanner->current = source;
}

static void skipWhitespace(Scanner* scanner);
static bool isAtEnd(Scanner scanner);
static char advance(Scanner* scanner);
static char peek(Scanner scanner);
static bool isDigit(char c);
static Token makeToken(Scanner scanner, TokenType type);
static Token errorToken(Scanner scanner, const char *message);
static Token number(Scanner *scanner);

Token scanToken(Scanner *scanner) {
  skipWhitespace(scanner);

  scanner->start = scanner->current;

  if (isAtEnd(*scanner)) {
    return makeToken(*scanner, TOKEN_EOF);
  }

  char c = advance(scanner);

  if (isDigit(c)) {
    return number(scanner);
  }

  switch (c) {
    case '(':
      return makeToken(*scanner, TOKEN_LEFT_PAREN);
    case ')':
      return makeToken(*scanner, TOKEN_RIGHT_PAREN);
    case '-':
      return makeToken(*scanner, TOKEN_MINUS);
    case '+':
      return makeToken(*scanner, TOKEN_PLUS);
    case '/':
      return makeToken(*scanner, TOKEN_SLASH);
    case '*':
      return makeToken(*scanner, TOKEN_STAR);
    default:
      return errorToken(*scanner, "Unexpected character.");
  }
}

static bool isAtEnd(Scanner scanner) {
  return *scanner.current == '\0';
}

static char advance(Scanner* scanner) {
  scanner->current++;
  return scanner->current[-1];
}

static char peek(Scanner scanner) {
  if (isAtEnd(scanner)) {
    return '\0';
  }
  return *scanner.current;
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static Token number(Scanner* scanner) {
  while (isDigit(peek(*scanner))) {
    advance(scanner);
  }
  return makeToken(*scanner, TOKEN_NUMBER);
}

static void skipWhitespace(Scanner* scanner) {
  for (;;) {
    char c = peek(*scanner);
    switch (c) {
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        advance(scanner);
        break;

      default:
        return;
    }
  }
}

static Token makeToken(Scanner scanner, TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  return token;
}

static Token errorToken(Scanner scanner, const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = strlen(message);
  return token;
}
