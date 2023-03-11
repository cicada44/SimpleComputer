#pragma once

#include <libreadkey/readkey.h>

#define MSG_INPUT_NUM "> "

// Big character.
typedef struct NUM {
    int N[2];
} NUM;

static NUM NUMS[]
        = {/* + */ {.N[0] = 0xFF181818, .N[1] = 0x181818FF},
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

/* Prints RAM in the box beginning in the (x1, y1) coordinate and
 * ending in the (x2, y2) coordinate. */
int output_memory_in_box(int x1, int y1, int x2, int y2);

/* Prints accumulator info. */
int output_accum();

/* Prints instructrion counter info. */
int output_instrcounter();

/* Prints operation info. */
int output_operation();

/* Prints flags info. */
int output_flags();

/* Prints keys info. */
int output_keys();

/* Prints sing (+ of -) before memory content. */
int print_mem_cell_sign();

/* Prints 1st number in memory cell. */
int print_mem_cell_1();

/* Prints 2nd number in memory cell. */
int print_mem_cell_2();

/* Prints 3st number in memory cell. */
int print_mem_cell_3();

/* Prints 4st number in memory cell. */
int print_mem_cell_4();

/* Prints whole memory cell. */
int print_MC();

/* Draw all interface frontend. */
void interface();

/* Processes the entered character. */
void process_key(enum keys* k);
