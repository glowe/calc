#include "debug.h"

#include <stdio.h>

static uint32_t simpleInstruction(const char* name, uint32_t offset) {
  printf("%s\n", name);
  return offset + 1;
}

static uint32_t constantInstruction(const char* name, Chunk* chunk,
                                    uint32_t offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '%f'\n", name, constant, chunk->constants.values[constant]);
  return offset + 2;
}

static uint32_t disassembleInstruction(Chunk* chunk, uint32_t offset) {
  printf("%04d ", offset);

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_ADD:
      return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
      return simpleInstruction("OP_SUBTRACT", offset);
    case OP_DIVIDE:
      return simpleInstruction("OP_DIVIDE", offset);
    case OP_MULTIPLY:
      return simpleInstruction("OP_MULTIPLY", offset);
    case OP_NEGATE:
      return simpleInstruction("OP_NEGATE", offset);
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    default:
      printf("Unhandled instruction: %d\n", instruction);
  }

  return offset + 1;
}

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("== %s ==\n", name);

  for (uint32_t offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}
