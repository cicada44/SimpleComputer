#include <SimpleComputer/front.h>
#include <common/common.h>
#include <ctype.h>
#include <fcntl.h>
#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Computer accumulator.
static int accumulator = 0;

// Computer instruction counter.
static int instruction_counter = 0;

// Actual operation.
static int actual_operation = 0;

// Numbers for RAM output.
NUM NUMS[] = {/* + */ {.N[0] = 0xFF181818, .N[1] = 0x181818FF},
              /* - */ {.N[0] = 0xFF000000, .N[1] = 0x000000FF},
              /* 0 */ {.N[0] = 0x8181817e, .N[1] = 0x7e818181},
              /* 1 */ {.N[0] = 0x8890A0C0, .N[1] = 0x80808080},
              /* 2 */ {.N[0] = 0x2040827C, .N[1] = 0xFE040810},
              /* 3 */ {.N[0] = 0x6080817E, .N[1] = 0x7E818060},
              /* 4 */ {.N[0] = 0xFF818181, .N[1] = 0x80808080},
              /* 5 */ {.N[0] = 0x7F0101FF, .N[1] = 0x7F808080},
              /* 6 */ {.N[0] = 0x0101817E, .N[1] = 0x7E81817F},
              /* 7 */ {.N[0] = 0x204080FE, .N[1] = 0x02040810},
              /* 8 */ {.N[0] = 0x7E81817E, .N[1] = 0x7E818181},
              /* 9 */ {.N[0] = 0x7E81817E, .N[1] = 0x7E808080},
              /* A */ {.N[0] = 0x7E42423C, .N[1] = 0x42424242},
              /* B */ {.N[0] = 0x3E42423E, .N[1] = 0x3E424242},
              /* C */ {.N[0] = 0x0101017E, .N[1] = 0x7E010101},
              /* D */ {.N[0] = 0x4242221E, .N[1] = 0x1E224242},
              /* E */ {.N[0] = 0x7E02027E, .N[1] = 0x7E020202},
              /* F */ {.N[0] = 0x7E02027E, .N[1] = 0x02020202}};

int output_memory_in_box(int x1, int y1, int x2, int y2) {
  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL) {
    return FAIL;
  }

  char buf[BUF_SIZE] = {};

  bc_box(x1, y1, x2, y2);

  int *tval = malloc(sizeof(int));

  // Goto next line...
  ++x1;

  for (size_t i = MIN_MEMORY_ADDRESS; i != MEMORY_SIZE / 10; ++i) {
    // Goto next line...
    if (mt_gotoXX(x1++, y1 + 1) == FAIL) {
      return FAIL;
    }

    // Print row of RAM.
    for (int j = MIN_MEMORY_ADDRESS; j != MEMORY_SIZE / 10; ++j) {
      if (sc_memoryGet(i * (MEMORY_SIZE / 10) + j, tval) == FAIL) {
        return FAIL;
      }

      if ((*tval & 0x4000) >> 14 == 1) {
        write(term, "-", 2);
        *tval >>= 1;
      } else {
        write(term, "+", 2);
      }

      sprintf(buf, "%04X ", *tval);

      write(term, buf, sizeof(buf));
    }

    bc_printNL();
  }

  free(tval);

  mt_gotoXX(1, 30);

  write(term, TITLE_MEMORY, sizeof(TITLE_MEMORY));

  close(term);

  return SUCCESS;
}

int output_accum() {
  if (bc_box(1, 63, 1, 20) == FAIL) {
    return FAIL;
  }

  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL || isatty(term) == 0) {
    return FAIL;
  }

  mt_gotoXX(1, 67);
  write(term, TITLE_ACCUM, sizeof(TITLE_ACCUM));

  int accum_value = accumulator;

  mt_gotoXX(2, 71);

  if ((accum_value & 0x4000) >> 14 == 1) {
    write(term, "-", 2);
    accum_value >>= 1;
  } else {
    write(term, "+", 2);
  }

  char buf[16] = {};
  sprintf(buf, "%04X", accum_value);
  write(term, buf, BUF_SIZE);

  close(term);

  return SUCCESS;
}

int output_instrcounter() {
  if (bc_box(4, 63, 1, 20) == FAIL) {
    return FAIL;
  }

  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL || isatty(term) == 0) {
    return FAIL;
  }

  // Print title.
  mt_gotoXX(4, 64);
  write(term, TITLE_COUNTER, sizeof(TITLE_COUNTER));

  mt_gotoXX(5, 71);

  if ((instruction_counter & 0x4000) >> 14 == 1) {
    write(term, MINUS, sizeof(MINUS));
    instruction_counter >>= 1;
  } else {
    write(term, PLUS, sizeof(PLUS));
  }

  char buf[BUF_SIZE] = {};
  sprintf(buf, "%04X", instruction_counter);
  write(term, buf, sizeof(buf));

  close(term);

  return SUCCESS;
}

int output_operation() {
  if (bc_box(7, 63, 1, 20) == FAIL) {
    return FAIL;
  }

  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL || isatty(term) == 0) {
    return FAIL;
  }

  mt_gotoXX(7, 68);
  write(term, TITLE_OPERATION, sizeof(TITLE_OPERATION));

  char buf[BUF_SIZE] = {};

  int *command = malloc(sizeof(int));
  int *operand = malloc(sizeof(int));

  sc_commandDecode(actual_operation, command, operand);

  sprintf(buf, "%02X : %02X", *command, *operand);

  mt_gotoXX(8, 69);

  if ((actual_operation & 0x4000) >> 14 == 1) {
    write(term, MINUS, sizeof(MINUS));
    instruction_counter >>= 1;
  } else {
    write(term, PLUS, sizeof(PLUS));
  }

  write(term, buf, BUF_SIZE);

  free(command);
  free(operand);

  close(term);

  return SUCCESS;
}

int output_flags() {
  if (bc_box(10, 63, 1, 20) == FAIL) {
    return FAIL;
  }

  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL || isatty(term) == 0) {
    return FAIL;
  }

  mt_gotoXX(10, 70);
  write(term, TITLE_FLAGS, sizeof(TITLE_FLAGS));

  int *value = malloc(sizeof(int));

  mt_gotoXX(11, 69);
  sc_regGet(FLAG_OVERFLOW_N, value);
  write(term, ((*value) ? FLAG_OVERFLOW : ""), 1);

  mt_gotoXX(11, 71);
  sc_regGet(FLAG_NULL_DIV_N, value);
  write(term, ((*value) ? FLAG_NULL_DIV : ""), 1);

  mt_gotoXX(11, 73);
  sc_regGet(FLAG_UNK_COMMAND_N, value);
  write(term, ((*value) ? FLAG_UNK_COMMAND : ""), 1);

  mt_gotoXX(11, 75);
  sc_regGet(FLAG_IGNORE_N, value);
  write(term, ((*value) ? FLAG_IGNORE : ""), 1);

  mt_gotoXX(11, 77);
  sc_regGet(FLAG_OUT_OF_MEM_N, value);
  write(term, ((*value) ? FLAG_OUT_OF_MEM : ""), 1);

  free(value);

  close(term);

  return SUCCESS;
}

int output_keys() {
  if (bc_box(13, 47, 8, 36) == FAIL) {
    return FAIL;
  }

  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL || isatty(term) == 0) {
    return FAIL;
  }

  mt_gotoXX(13, 48);

  write(term, TITLE_KEYS, sizeof(TITLE_KEYS));

  // Writes titles keys.
  mt_gotoXX(14, 48);
  write(term, TITLE_KEYS_LOAD, sizeof(TITLE_KEYS_LOAD));
  mt_gotoXX(15, 48);
  write(term, TITLE_KEYS_SAVE, sizeof(TITLE_KEYS_SAVE));
  mt_gotoXX(16, 48);
  write(term, TITLE_KEYS_RUN, sizeof(TITLE_KEYS_RUN));
  mt_gotoXX(17, 48);
  write(term, TITLE_KEYS_STEP, sizeof(TITLE_KEYS_STEP));
  mt_gotoXX(18, 48);
  write(term, TITLE_KEYS_RESET, sizeof(TITLE_KEYS_RESET));
  mt_gotoXX(19, 48);
  write(term, TITLE_KEYS_ACCUM, sizeof(TITLE_KEYS_ACCUM));
  mt_gotoXX(20, 48);
  write(term, TITLE_KEYS_INSTRCNTER, sizeof(TITLE_KEYS_INSTRCNTER));

  close(term);

  return SUCCESS;
}

int print_mem_cell_sign(NUM a) { return bc_printbigchar(a.N, 14, 2, 10, 44); }

int print_mem_cell_1(NUM a) { return bc_printbigchar(a.N, 14, 11, 10, 44); }

int print_mem_cell_2(NUM a) { return bc_printbigchar(a.N, 14, 20, 10, 44); }

int print_mem_cell_3(NUM a) { return bc_printbigchar(a.N, 14, 29, 10, 44); }

int print_mem_cell_4(NUM a) { return bc_printbigchar(a.N, 14, 38, 10, 44); }

int print_MC(int n) {
  bc_box(13, 1, 8, 44);

  int *num = malloc(sizeof(int));
  sc_memoryGet(n, num);

  if (*num >> 14 == 1) {
    print_mem_cell_sign(NUMS[1]);
    *num >>= 1;
  } else {
    print_mem_cell_sign(NUMS[0]);
  }

  print_mem_cell_4(NUMS[(*num & 0xf) + 2]);
  print_mem_cell_3(NUMS[((*num >> 4) & 0xf) + 2]);
  print_mem_cell_2(NUMS[((*num >> 8) & 0xf) + 2]);
  print_mem_cell_1(NUMS[((*num >> 12) & 0xf) + 2]);

  free(num);

  return SUCCESS;
}

void interface() {
  int current_command = 0;

  sc_memoryInit();

  mt_clrscr();

  output_memory_in_box(1, 1, 10, 60);

  output_accum();

  output_instrcounter(current_command);

  output_operation();

  output_flags();

  output_keys();

  print_MC(0);

  mt_gotoXX(30, 0);

  sc_memoryDelete();
}
