//-----------------------------------------------------------------------------
//
// Header for libbigchar.a library. Contains functions that process 'BigChar'
// type of data.
//
//-----------------------------------------------------------------------------

#pragma once

#include <libterm/term.h>

/* Prints string of extended ascii symbols to actual terminal. */
int bc_printA(char* str);

/* Prints newline character to actual terminal. */
int bc_printNL();

// Box output functions.

/* Prints box upper bound to actual terminal.  */
int bc_printUB(int len);

/* Prints box lower bound to actual terminal.  */
int bc_printLB(int len);

/* Prints box upper bound to actual terminal.  */
int bc_printES(int len);

/* Prints box from (x1, y1) to (x2, y2). */
int bc_box(int x1, int y1, int x2, int y2);

/* Prints big character with c_front and c_back color to actual terminal. */
int bc_printbigchar(int a[2], int x, int y, enum color c_front, enum color c_back);

/* Sets bit in coordinate (x, y) in bigchar to value. */
int bc_setbigcharpos(int* big, int x, int y, int value);

/* Gets bit in coordinate (x, y) in bigchar and write it to value. */
int bc_getbigcharpos(int* big, int x, int y, int* value);

/* Writes count bigchars into descriptor with fd code. */
int bc_bigcharwrite(int fd, int* big, int count);

/*  Reades count bigchars from descriptor with fd code.
    Return FAIL if read count of bigchars not equal to count.  */
int bc_bigcharread(int fd, int* big, int need_count, int* count);
