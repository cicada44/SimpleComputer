#pragma once

enum color { BLACK = 30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };

int mt_clrscr();
int mt_gotoXX(int x, int y);
int mt_getscreensize(int* rows, int* cols);
int mt_setfgcolor(enum color c);
int mt_setbgcolor(enum color c);
