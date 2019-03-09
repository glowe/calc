#include <stdio.h>

#include "chunk.h"
#include "compiler.h"
#include "debug.h"
#include "vm.h"

static void run() {
  char line[2048];
  VM vm;
  initVM(&vm);

  for (;;) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }
    Chunk chunk;
    initChunk(&chunk);
    compile(line, &chunk);
    disassembleChunk(&chunk, "run");
    interpret(&vm, &chunk);
  }
}

int main(int argc, char* argv[]) {
  run();

  return 0;
}
