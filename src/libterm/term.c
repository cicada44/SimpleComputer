#include <fcntl.h>
#include <libcommon/common.h>
#include <libterm/term.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Difference between foreground and background colors
   in escape sequence. */
#define DIFF_BG_COLOR 10

/* Lenght of the escape sequence of color changing. */
#define NORMAL_COLOR_SIZE 6

/* Start position for output. */
#define START_POS 0

/* Escape sequences. */
#define CLEAR_TERM "\E[H\E[J\n"
#define RESET_COLOR "\033[0m"

int mt_open()
{
    setvbuf(stdout, NULL, _IONBF, 0); /* Turn off buffering. */

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        runtime_error_process(RE.ERROR_OPENING_TERM);
        exit(FAIL);
    }

    return term;
}

int mt_clrscr()
{
    int term = mt_open();

    char buf[BUF_SIZE] = CLEAR_TERM;

    write(term, buf, BUF_SIZE);

    mt_gotoXX(START_POS, START_POS);

    close(term);

    return SUCCESS;
}

int mt_gotoXX(int x, int y)
{
    if (x < START_POS || y < START_POS) {
        runtime_error_process(RE.ERROR_ANY_TERM);
        return FAIL;
    }

    int term = mt_open();

    char buf[BUF_SIZE] = {};

    sprintf(buf, "\033[%d;%df", x, y);

    write(term, buf, BUF_SIZE);

    close(term);

    return SUCCESS;
}

int mt_getscreensize(int* rows, int* cols)
{
    struct winsize ws;

    if (rows == NULL || cols == NULL
        || ioctl(TERMINAL_OPENCODE, TIOCGWINSZ, &ws)) {
        runtime_error_process(RE.ERROR_ANY_TERM);
        return FAIL;
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;

    return SUCCESS;
}

int mt_setfgcolor(enum color c)
{
    int term = mt_open();

    char buf[BUF_SIZE] = {};

    sprintf(buf, "%s%d%s", "\e[", c, "m");

    if (write(term, buf, NORMAL_COLOR_SIZE) == FAIL) {
        runtime_error_process(RE.ERROR_ANY_TERM);
        return FAIL;
    }

    close(term);

    return SUCCESS;
}

int mt_setbgcolor(enum color c)
{
    int term = mt_open();

    char buf[BUF_SIZE] = {};

    sprintf(buf, "%s%d%s", "\e[", c + DIFF_BG_COLOR, "m");

    if (write(term, buf, NORMAL_COLOR_SIZE) == FAIL) {
        runtime_error_process(RE.ERROR_ANY_TERM);
        return FAIL;
    }

    close(term);

    return SUCCESS;
}

int mt_resetcolor()
{
    int term = mt_open();

    char buf[BUF_SIZE] = {};

    sprintf(buf, "%s", RESET_COLOR);

    if (write(term, buf, BUF_SIZE) == FAIL) {
        runtime_error_process(RE.ERROR_ANY_TERM);
        return FAIL;
    }

    close(term);

    return SUCCESS;
}
