#ifndef calc_value_h
#define calc_value_h

#include "common.h"

typedef struct {
  uint8_t count;
  uint8_t capacity;
  double* values;
} ValueArray;

void initValueArray(ValueArray* array);
void freeValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, double value);

#endif
