//-----------------------------------------------------------------------------
//
// Header for libterm.a library. Contains functions for working terminal
// processing.
//
//-----------------------------------------------------------------------------

#pragma once

// Color codes for terminal.
enum color { BLACK = 30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

/* Open standard system terminal. Returns terminal descriptor number.
   If there isn't any openable temrinal in system - terminate with code -1. */
int mt_open();

/* Clears the console */
int mt_clrscr();

/* Sets cursor to (x, y) position. */
int mt_gotoXX(int x, int y);

/* Gets actual terminal size and writes (x, y) to (rows, cols). */
int mt_getscreensize(int* rows, int* cols);

/* Sets foreground terminal color. */
int mt_setfgcolor(enum color c);

/* Sets background terminal color. */
int mt_setbgcolor(enum color c);

/* Discard actual terminal color. */
int mt_resetcolor();
