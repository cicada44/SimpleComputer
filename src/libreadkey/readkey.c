#include <common/common.h>
#include <fcntl.h>
#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libreadkey/readkey.h>
#include <libterm/term.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// Terminal dettings file.
#define TERM_STATE_FILE "termsettings"

// All readable keys.
#define KEYS "lsrti"
#define F5_KEY "\E[15~"
#define F6_KEY "\E[17~"
#define DOWN_KEY "\E[B"
#define UP_KEY "\E[A"
#define LEFT_KEY "\E[D"
#define RIGHT_KEY "\E[C"
#define ENTER_KEY "\n"

#define BUF_LEN_MEM_KEYS 3
#define BUF_LEN_F 4

int rk_readkey(enum keys* k)
{
    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        fprintf(stderr, "FAIL OPENING TERMINAL");
        exit(FAIL);
    }

    // Modifying terminal.
    setvbuf(stdout, NULL, _IONBF, 0);
    struct termios actual_term_set;
    tcgetattr(0, &actual_term_set);
    actual_term_set.c_lflag |= ISIG;
    actual_term_set.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &actual_term_set);

    char buf[BUF_SIZE];
    read(0, buf, sizeof(buf));

    // Check the containing key.
    char key = buf[0];

    if (key == 'l') {
        *k = LOAD;
    } else if (key == 's') {
        *k = SAVE;
    } else if (key == 'r') {
        *k = RUN;
    } else if (key == 't') {
        *k = STEP;
    } else if (key == 'i') {
        *k = RESET;
    } else if (strncmp(buf, F5_KEY, BUF_LEN_F) == 0) {
        *k = F5;
    } else if (strncmp(buf, F6_KEY, BUF_LEN_F) == 0) {
        *k = F6;
    } else if (strncmp(buf, DOWN_KEY, BUF_LEN_MEM_KEYS) == 0) {
        *k = DOWN;
    } else if (strncmp(buf, UP_KEY, BUF_LEN_MEM_KEYS) == 0) {
        *k = UP;
    } else if (strncmp(buf, LEFT_KEY, BUF_LEN_MEM_KEYS) == 0) {
        *k = LEFT;
    } else if (strncmp(buf, RIGHT_KEY, BUF_LEN_MEM_KEYS) == 0) {
        *k = RIGHT;
    } else if (strncmp(buf, ENTER_KEY, 1) == 0) {
        *k = ENTER;
    } else {
        *k = OTHER;
    }

    close(term);

    return FAIL;
}

int rk_termsave()
{
    FILE* termstate_f;

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        return FAIL;
    }

    termstate_f = fopen(TERM_STATE_FILE, WRITE_BIN);

    if (termstate_f == NULL) {
        close(term);
        return FAIL;
    }

    struct termios actual_term_set;

    if (fwrite(&actual_term_set, sizeof(actual_term_set), 1, termstate_f)
        != sizeof(actual_term_set)) {
        close(term);
        return FAIL;
    }

    close(term);

    return SUCCESS;
}
