#pragma once

enum colors { BLACK = 30, RED, GREEN, YELLOW };

int mt_clrscr();
int mt_gotoXX(int x, int y);
int mt_getscreensize(int* rows, int* cols);
int mt_setfgcolor(enum colors c);
// int mt_setbgcolor(enum color c);
