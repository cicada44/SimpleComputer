#include <common/common.h>
#include <libcomputer/comp.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#define ZERO_BIT 0x0
#define ONE_BIT 0x1
#define MASK_DECODE_COMMAND 0x7f

static int flags = 0;

static int* memory = NULL;

static int commands[] = {
    10, 11, 20, 21, 30, 31, 32, 33, 40, 41, 42, 43, 51, 52, 53, 54, 55, 56, 57,
    58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
};

int sc_memoryInit() {
  memory = calloc(100, sizeof(int));

  if (memory == NULL) {
    return FAIL_CODE;
  }

  for (int x = 0; x != 100; ++x) {
    sc_memorySet(x, 0);
  }

  return SUCCESS_CODE;
}

int sc_memorySet(int address, int value) {
  if (address < 0 || address > 99) {
    sc_regSet(3, 1);
    return FAIL_CODE;
  }

  memory[address] = value;

  return SUCCESS_CODE;
}

int sc_memoryGet(int address, int* value) {
  if (value == NULL || address < 0 || address > 99) {
    sc_regSet(3, 1);
    return FAIL_CODE;
  }

  *value = memory[address];

  return SUCCESS_CODE;
}

int sc_memorySave(char* filename) {
  FILE* output_file = fopen(filename, "wb");

  if (output_file == NULL || memory == NULL) {
    return FAIL_CODE;
  }

  if (fwrite(memory, sizeof(int), 100, output_file) != 100) {
    fclose(output_file);
    return FAIL_CODE;
  }

  fclose(output_file);

  return SUCCESS_CODE;
}

int sc_memoryLoad(char* filename) {
  FILE* input_file = fopen(filename, "rb");

  if (input_file == NULL || memory == NULL) {
    return FAIL_CODE;
  }

  if (fread(memory, sizeof(int), 100, input_file) != 100) {
    fclose(input_file);
    return FAIL_CODE;
  }

  fclose(input_file);

  return SUCCESS_CODE;
}

int sc_regInit() {
  flags = 0;
  return SUCCESS_CODE;
}

int sc_regSet(int reg, int value) {
  if (reg < 1 || reg > 5 || (value != 0 && value != 1)) {
    return FAIL_CODE;
  }

  (value == 1) ? (flags |= (1 << (reg - 1))) : (flags &= (~(1 << (reg - 1))));

  return SUCCESS_CODE;
}

int sc_regGet(int reg, int* value) {
  if (reg < 1 || reg > 5 || value == NULL) {
    return FAIL_CODE;
  }

  *value = (flags >> (reg - 1)) & ONE_BIT;

  return SUCCESS_CODE;
}

int sc_commandEncode(int command, int operand, int* value) {
  if (bsearch(&command, commands, 38, sizeof(int), comp) == NULL ||
      operand > 0x7f || command > 0x7f || value == NULL) {
    return FAIL_CODE;
  }

  *value = ZERO_BIT;

  *value = command << 7;
  *value |= operand;

  return SUCCESS_CODE;
}

int sc_commandDecode(int value, int* command, int* operand) {
  if (command == NULL || operand == NULL) {
    sc_regSet(5, 1);
    return FAIL_CODE;
  }

  *operand = *command = ZERO_BIT;

  *operand = value & MASK_DECODE_COMMAND;
  value >>= 7;
  *command = value;

  return SUCCESS_CODE;
}

int sc_memoryDelete() {
  if (memory == NULL) {
    return FAIL_CODE;
  }
  free(memory);

  return SUCCESS_CODE;
}

void output_memory() {
  for (size_t i = 0; i != 100; ++i) {
    if (i % 10 == 0 && i != 0) {
      printf("\n");
    }
    printf("%3d ", memory[i]);
  }

  printf("\n");
}

void bin(unsigned n) {
  unsigned i;
  for (i = 1 << 31; i > 0; i = i / 2) (n & i) ? printf("1") : printf("0");
}

int comp(const void* n1, const void* n2) { return (*(int*)n1 - *(int*)n2); }
