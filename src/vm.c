#include <stdio.h>

#include "value.h"
#include "vm.h"

static void resetStack(VM* vm) { vm->stackTop = vm->stack; }

void initVM(VM* vm) {
  resetStack(vm);
  vm->chunk = NULL;
  vm->ip = NULL;
}

static void push(VM* vm, double value) {
  *vm->stackTop = value;
  vm->stackTop++;
}

static double pop(VM* vm) {
  vm->stackTop--;
  return *vm->stackTop;
}

InterpretResult interpret(VM* vm, Chunk* chunk) {
#define READ_BYTE() (*vm->ip++)
#define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)   \
  do {                  \
    double b = pop(vm); \
    double a = pop(vm); \
    push(vm, a op b);   \
  } while (0)

  vm->chunk = chunk;
  vm->ip = chunk->code;

  for (;;) {
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        double constant = READ_CONSTANT();
        push(vm, constant);
        break;
      }
      case OP_MULTIPLY:
        BINARY_OP(*);
        break;
      case OP_DIVIDE:
        BINARY_OP(/);
        break;
      case OP_ADD:
        BINARY_OP(+);
        break;
      case OP_SUBTRACT:
        BINARY_OP(-);
        break;
      case OP_NEGATE:
        push(vm, -1 * pop(vm));
        break;
      case OP_RETURN: {
        double value = pop(vm);
        printf("%f\n", value);
        return INTERPRET_OK;
      }
    }
  }
  // Control should not reach here.
  return INTERPRET_RUNTIME_ERROR;

#undef BINARY_OP
#undef READ_CONSTANT
#undef READ_BYTE
}
