#include <libbigchar/bigchar.h>
#include <libcommon/common.h>
#include <libcomputer/comp.h>
#include <libreadkey/readkey.h>
#include <libterm/term.h>

#include <fcntl.h>
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
    int term = mt_open();

    /* Modifying terminal. */
    setvbuf(stdout, NULL, _IONBF, 0);
    struct termios actual_term_set;
    tcgetattr(0, &actual_term_set);
    actual_term_set.c_lflag |= ISIG;
    actual_term_set.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &actual_term_set);

    char buf[BUF_SIZE];
    read(0, buf, sizeof(buf));

    /* Check the containing key. */
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

int rk_termsave(const char* const file_name)
{
    FILE* termstate_f;

    int term = mt_open();

    termstate_f = fopen(file_name, WRITE_BIN);

    if (termstate_f == NULL) {
        runtime_error_process(RE.ERROR_OPENING_FILE);
        close(term);
        return FAIL;
    }

    struct termios actual_term_set;

    if (fwrite(&actual_term_set, sizeof(actual_term_set), 1, termstate_f) != 1) {
        fclose(termstate_f);
        runtime_error_process(RE.ERROR_FILE_WRITING);
        close(term);
        return FAIL;
    }

    fclose(termstate_f);

    close(term);

    return SUCCESS;
}

int rk_termrestore(const char* const file_name)
{
    int term = mt_open();
    FILE* termstate_f;
    termstate_f = fopen(file_name, READ_BIN);

    if (termstate_f == NULL) {
        runtime_error_process(RE.ERROR_OPENING_FILE);
        close(term);
        return FAIL;
    }

    struct termios actual_term_set;

    if (fread(&actual_term_set, sizeof(actual_term_set), 1, termstate_f) != 1) {
        fclose(termstate_f);
        runtime_error_process(RE.ERROR_FILE_READING);
        close(term);
        return FAIL;
    }

    tcsetattr(0, TCSANOW, &actual_term_set);

    fclose(termstate_f);
    close(term);

    return SUCCESS;
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        runtime_error_process(RE.ERROR_OPENING_TERM);
        exit(FAIL);
    }

    struct termios actual_term;

    tcgetattr(0, &actual_term);

    if (regime == 0) {
        actual_term.c_lflag &= ~ICANON;
    } else if (regime == 1) {
        actual_term.c_lflag |= ICANON;

        if (vtime == 1) {
            actual_term.c_lflag |= VTIME;
        } else if (vtime == 0) {
            actual_term.c_lflag &= ~VTIME;
        } else {
            runtime_error_process(RE.ERROR_TERM_OPT);
            return FAIL;
        }

        if (vmin == 1)
            actual_term.c_lflag |= VMIN;
        else if (vmin == 0) {
            actual_term.c_lflag &= ~VMIN;
        } else {
            runtime_error_process(RE.ERROR_TERM_OPT);
            return FAIL;
        }

        if (echo == 1)
            actual_term.c_lflag |= ECHO;
        else if (echo == 0)
            actual_term.c_lflag &= ~ECHO;
        else {
            runtime_error_process(RE.ERROR_TERM_OPT);
            return FAIL;
        }

        if (sigint == 1)
            actual_term.c_lflag |= ISIG;
        else if (sigint == 0)
            actual_term.c_lflag &= ~ISIG;
        else {
            runtime_error_process(RE.ERROR_TERM_OPT);
            return FAIL;
        }
    } else {
        runtime_error_process(RE.ERROR_TERM_OPT);
        return FAIL;
    }

    close(term);

    return SUCCESS;
}
