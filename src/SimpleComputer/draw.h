//-----------------------------------------------------------------------------
//
// Header for drawing interface. Contains functions for interface viewing.
//
//-----------------------------------------------------------------------------

#pragma once

#include <libreadkey/readkey.h>

#define MSG_INPUT "> "

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

/* Prints IOfield. */
int output_iofield();

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

/* Draws all gui elements. */
void GUI();
