#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
  Scanner scanner;
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
  Chunk* chunk;
} Parser;

static void errorAtCurrent(Parser* parser, const char* message) {
  if (parser->panicMode) {
    return;
  }
  parser->panicMode = true;

  Token token = parser->current;
  fprintf(stderr, "Error ");

  if (token.type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token.type == TOKEN_ERROR) {
    // Nothing.
  } else {
    fprintf(stderr, "at '%.*s'", token.length, token.start);
  }

  fprintf(stderr, ": %s\n", message);
  parser->hadError = true;
}

static void advance(Parser* parser) {
  for (;;) {
    parser->previous = parser->current;
    parser->current = scanToken(&parser->scanner);
    if (parser->current.type != TOKEN_ERROR) {
      break;
    }
    errorAtCurrent(parser, parser->current.start);
  }
}

static bool match(Parser* parser, TokenType type) {
  if (parser->current.type != type) {
    return false;
  }
  advance(parser);
  return true;
}

static bool match2(Parser* parser, TokenType firstType, TokenType secondType) {
  if (parser->current.type == firstType || parser->current.type == secondType) {
    advance(parser);
    return true;
  }
  return false;
}

static void consume(Parser* parser, TokenType type, const char* message) {
  if (parser->current.type == type) {
    advance(parser);
    return;
  }
  errorAtCurrent(parser, message);
}

static Token peek(Parser* parser) { return parser->current; }

static Token previous(Parser* parser) { return parser->previous; }

static void emitByte(Parser* parser, uint8_t byte) {
  writeChunk(parser->chunk, byte);
}

static void emitConstant(Parser* parser, double value) {
  emitByte(parser, OP_CONSTANT);
  uint8_t index = addConstant(parser->chunk, value);
  writeChunk(parser->chunk, index);
}

static void expression(Parser* parser);

static void primary(Parser* parser) {
  if (peek(parser).type == TOKEN_NUMBER) {
    consume(parser, TOKEN_NUMBER, "Expected number.");
    Token token = previous(parser);
    double value = strtod(token.start, NULL);
    emitConstant(parser, value);
  } else {
    consume(parser, TOKEN_LEFT_PAREN, "Expect '('.");
    expression(parser);
    consume(parser, TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
  }
}

static void unary(Parser* parser) {
  if (match(parser, TOKEN_MINUS)) {
    unary(parser);
    emitByte(parser, OP_NEGATE);
  } else {
    primary(parser);
  }
}

static void multiplication(Parser* parser) {
  unary(parser);

  while (match2(parser, TOKEN_SLASH, TOKEN_STAR)) {
    Token token = previous(parser);
    unary(parser);
    OpCode code = token.type == TOKEN_SLASH ? OP_DIVIDE : OP_MULTIPLY;
    emitByte(parser, code);
  }
}

static void addition(Parser* parser) {
  multiplication(parser);

  while (match2(parser, TOKEN_MINUS, TOKEN_PLUS)) {
    Token token = previous(parser);
    multiplication(parser);
    OpCode code = token.type == TOKEN_MINUS ? OP_SUBTRACT : OP_ADD;
    emitByte(parser, code);
  }
}

static void expression(Parser* parser) {
  addition(parser);
}

// Grammar:
// 
// expression     -> addition ;
// addition       -> multiplication (( "-" | "+" ) multiplication)* ;
// multiplication -> unary (( "*" | "/" ) unary)* ;
// unary          -> "-" unary | primary
// primary        -> NUMBER | "(" expression ")";

bool compile(const char* source, Chunk* chunk) {
  Parser parser;
  initScanner(&parser.scanner, source);
  parser.chunk = chunk;
  parser.hadError = false;
  parser.panicMode = false;
  advance(&parser);

  while (!match(&parser, TOKEN_EOF)) {
    expression(&parser);
  }
  emitByte(&parser, OP_RETURN);

  return !parser.hadError;
}
