#include <libterm/term.h>

#include <common/common.h>

#include <sys/ioctl.h>

#include <stdio.h>

#include <unistd.h>

#define DIFF_BG_COLOR 10

int mt_clrscr()
{
    if (printf("\E[H\E[J") != 1) {
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}

int mt_gotoXX(int x, int y)
{
    if (x < 0 || y < 0) {
        return FAIL_CODE;
    }

    printf("\e[10C");

    return SUCCESS_CODE;
}

int mt_getscreensize(int* rows, int* cols)
{
    struct winsize ws;

    if (rows == NULL || cols == NULL || ioctl(1, TIOCGWINSZ, &ws)) {
        return FAIL_CODE;
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;

    return SUCCESS_CODE;
}

int mt_setfgcolor(enum color c)
{
    if (printf("\x1b[") != 2 || printf("%d", c) != 2 || printf("m") != 1) {
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}

int mt_setbgcolor(enum color c)
{
    if (printf("\x1b[") != 2 || printf("%d", c + DIFF_BG_COLOR) != 2
        || printf("m") != 1) {
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}
