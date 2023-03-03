#include <common/common.h>
#include <fcntl.h>
#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libreadkey/readkey.h>
#include <libterm/term.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// Terminal dettings file.
#define TERM_STATE_FILE "termsettings"

// All readable keys.
#define KEYS "lsrti"
#define F5_KEY "\E[[E"
#define F6_KEY "\E[17~"
#define DOWN_KEY "\E[B"
#define UP_KEY "\E[A"
#define LEFT_KEY "\E[D"
#define RIGHT_KEY "\033\133\103\n"

int rk_readkey(enum keys* k)
{
    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        fprintf(stderr, "FAIL OPENING TERMINAL");
        return FAIL;
    }

    char buf[5];

    read(0, buf, 5);

    for (size_t x = 0; x != sizeof(buf); ++x) {
        printf("%ld - %c\n", x, buf[x]);
    }

    // if (strcmp(buf, "\E[C") == 0) {
    // printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    // }

    // Check the containing key.

    char key = buf[0];

    if (key == 'l') {
        *k = LOAD;
        close(term);
        return SUCCESS;
    }

    if (key == 's') {
        *k = SAVE;
        close(term);
        return SUCCESS;
    }

    if (key == 'r') {
        *k = RUN;
        close(term);
        return SUCCESS;
    }

    if (key == 't') {
        *k = STEP;
        close(term);
        return SUCCESS;
    }

    if (key == 'i') {
        *k = RESET;
        close(term);
        return SUCCESS;
    }

    if (strcmp(buf, F5_KEY) == 0) {
        *k = F5;
        close(term);
        return SUCCESS;
    }

    if (strcmp(buf, F6_KEY) == 0) {
        *k = F6;
        close(term);
        return SUCCESS;
    }

    if (strcmp(buf, DOWN_KEY) == 0) {
        *k = DOWN;
        close(term);
        return SUCCESS;
    }

    if (strcmp(buf, UP_KEY) == 0) {
        *k = UP;
        close(term);
        return SUCCESS;
    }

    if (strcmp(buf, LEFT_KEY) == 0) {
        *k = LEFT;
        close(term);
        return SUCCESS;
    }

    if (strcmp(buf, RIGHT_KEY) == 0) {
        *k = RIGHT;
        close(term);
        return SUCCESS;
    }

    close(term);

    printf("UNKNOWN\n");
    sleep(1);

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
        return FAIL;
    }

    struct termios actual_term_set;

    if (fwrite(&actual_term_set, sizeof(actual_term_set), 1, termstate_f)
        != sizeof(actual_term_set)) {
        return FAIL;
    }

    return SUCCESS;
}
