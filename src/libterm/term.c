#include <common/common.h>
#include <fcntl.h>
#include <libterm/term.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DIFF_BG_COLOR 10

#define NORMAL_COLOR_SIZE 6
#define BUF_SIZE 15

int mt_clrscr()
{
    int term = open(TERM_PATH, O_WRONLY);

    if (term == -1) {
        fprintf(stderr, "[E] Error opening terminal\n");
        return FAIL_CODE;
    }

    char buf[BUF_SIZE] = "\E[H\E[J";

    write(term, buf, sizeof(buf));

    close(term);

    return SUCCESS_CODE;
}

int mt_gotoXX(int x, int y)
{
    if (mt_clrscr() == FAIL_CODE) {
        return FAIL_CODE;
    }

    if (x < 0 || y < 0) {
        return FAIL_CODE;
    }

    int term = open(TERM_PATH, O_WRONLY);

    if (term == -1) {
        close(term);
        return FAIL_CODE;
    }

    char buf[BUF_SIZE];

    sprintf(buf, "\033[%d;%df", x, y);

    write(term, buf, 10);

    close(term);

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
    int term = open(TERM_PATH, O_WRONLY);

    if (term == -1) {
        close(term);
        return FAIL_CODE;
    }

    char buf[BUF_SIZE];

    sprintf(buf, "%s%d%s", "\e[", c, "m");

    write(term, buf, NORMAL_COLOR_SIZE);

    close(term);

    return SUCCESS_CODE;
}

int mt_setbgcolor(enum color c)
{
    int term = open(TERM_PATH, O_WRONLY);

    if (term == -1) {
        close(term);
        return FAIL_CODE;
    }

    char buf[BUF_SIZE];

    sprintf(buf, "%s%d%s", "\e[", c + DIFF_BG_COLOR, "m");

    write(term, buf, NORMAL_COLOR_SIZE);

    close(term);

    return SUCCESS_CODE;
}

int mt_resetcolor()
{
    int term = open(TERM_PATH, O_WRONLY);

    if (term == -1) {
        close(term);
        return FAIL_CODE;
    }

    char buf[BUF_SIZE];

    sprintf(buf, "%s", RESET_COLOR);

    write(term, buf, NORMAL_COLOR_SIZE);

    close(term);

    return SUCCESS_CODE;
}
