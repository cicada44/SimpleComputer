/*  Library for working with terminal.
    All functions return SUCCESS or FAIL (defined in common.h).  */

#pragma once

// Color codes for terminal.
enum color { BLACK = 30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

/* Cleares the console */
int mt_clrscr();

/* Sets cursor to (x, y) position. */
int mt_gotoXX(int x, int y);

/* Gets actual terminal size and writes (x, y) to (rows, cols). */
int mt_getscreensize(int *rows, int *cols);

/* Sets foreground terminal color. */
int mt_setfgcolor(enum color c);

/* Sets background terminal color. */
int mt_setbgcolor(enum color c);

/* Discard actual terminal color. */
int mt_resetcolor();

/* Returnes accumulator value. */
int get_accum_value();
