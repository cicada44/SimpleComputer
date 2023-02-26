/*
    Terminal library.
    All functions return SUCCESS or FAIL (defined in common.h).
*/

#include <common/common.h>
#include <fcntl.h>
#include <libterm/term.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Difference between foregr. and backgr. colors
// in escape sequence.
#define DIFF_BG_COLOR 10

// Lenght of the escape sequence of color changing.
#define NORMAL_COLOR_SIZE 6

// Start position.
#define START_POS 0

// Escape sequences.
#define CLEAR_TERM "\E[H\E[J\n"
#define RESET_COLOR "\033[0m"

// Clears the console.
int mt_clrscr()
{
    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    char buf[BUF_SIZE] = CLEAR_TERM;

    write(term, buf, BUF_SIZE);

    mt_gotoXX(START_POS, START_POS);

    close(term);

    return SUCCESS;
}

// Sets up position to the (x, y) coordinate.
int mt_gotoXX(int x, int y)
{
    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    if (x < START_POS || y < START_POS) {
        return FAIL;
    }

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    char buf[BUF_SIZE] = {};

    sprintf(buf, "\033[%d;%df", x, y);

    write(term, buf, BUF_SIZE);

    close(term);

    return SUCCESS;
}

// Write terminal scale to rows, cols.
int mt_getscreensize(int* rows, int* cols)
{
    struct winsize ws;

    if (rows == NULL || cols == NULL
        || ioctl(TERMINAL_OPENCODE, TIOCGWINSZ, &ws)) {
        return FAIL;
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;

    return SUCCESS;
}

// Sets the foreground color of the output.
int mt_setfgcolor(enum color c)
{
    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        close(term);
    }

    char buf[BUF_SIZE] = {};

    sprintf(buf, "%s%d%s", "\e[", c, "m");

    write(term, buf, NORMAL_COLOR_SIZE);

    close(term);

    return SUCCESS;
}

// Sets the background color of the output.
int mt_setbgcolor(enum color c)
{
    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    char buf[BUF_SIZE] = {};

    sprintf(buf, "%s%d%s", "\e[", c + DIFF_BG_COLOR, "m");

    write(term, buf, NORMAL_COLOR_SIZE);

    close(term);

    return SUCCESS;
}

// Resets color of the output.
int mt_resetcolor()
{
    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == -1) {
        return FAIL;
    }

    char buf[BUF_SIZE] = {};

    sprintf(buf, "%s", RESET_COLOR);

    write(term, buf, BUF_SIZE);

    close(term);

    return SUCCESS;
}
