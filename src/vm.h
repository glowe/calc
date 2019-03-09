#ifndef calc_vm_h
#define calc_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
  Chunk* chunk;
  uint8_t* ip;
  double stack[STACK_MAX];
  double* stackTop;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM(VM* vm);
InterpretResult interpret(VM* vm, Chunk* chunk);

#endif
