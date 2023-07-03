#include <libbigchar/bigchar.h>
#include <libcommon/common.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Extended ascii chars. */
#define RECT "a"
#define HR "q"
#define VT_BOUND "x"
#define RB "j"
#define RU "k"
#define LU "l"
#define LB "m"

/* 1st and 2nd ints in bigchar array. */
#define BC_FIRST_N 0
#define BC_SECOND_N 1

/* Min line number in bigchar array. */
#define MIN_LINE_N 1
/* Max line number in bigchar array. */
#define MAX_LINE_N 8

int bc_printNL()
{
    int term = mt_open();
    assert(write(term, NEWLINE, strlen(NEWLINE)) != FAIL);
    close(term);

    return SUCCESS;
}

int bc_printA(char* str)
{
    int term = mt_open();
    char buf[strlen(str)];
    sprintf(buf, "\e(0%s\e(B", str);
    assert(write(term, buf, strlen("\e(0%s\e(B")) != FAIL);
    close(term);

    return SUCCESS;
}

int bc_printUB(int len)
{
    assert(bc_printA(LU) != FAIL);
    for (int i = 0; i != len; ++i) {
        assert(bc_printA(HR) != FAIL);
    }
    assert(bc_printA(RU) != FAIL);
    assert(bc_printNL() != FAIL);
    return SUCCESS;
}

int bc_printLB(int len)
{
    assert(bc_printA(LB) != FAIL);
    for (int i = 0; i != len; ++i) {
        assert(bc_printA(HR) != FAIL);
    }
    assert(bc_printA(RB) != FAIL);
    assert(bc_printNL() != FAIL);
    return SUCCESS;
}

int bc_printES(int len)
{
    int term = mt_open();

    for (int i = 0; i != len; ++i) {
        write(term, " ", 2);
    }

    close(term);

    return SUCCESS;
}

int bc_box(int x1, int y1, int x2, int y2)
{
    setvbuf(stdout, NULL, _IONBF, 0); /* Turn off buffering. */

    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || mt_gotoXX(x1, y1) == FAIL
        || bc_printUB(y2) == FAIL) {
        runtime_error_process(RE.ERROR_ANY_BC);
        return FAIL;
    }

    ++x1; /* Goto next line... */

    for (int i = 0; i != x2; ++i) {
        mt_gotoXX(x1++, y1);
        assert(bc_printA(VT_BOUND) != FAIL);
        bc_printES(y2);
        assert(bc_printA(VT_BOUND) != FAIL);
        bc_printNL();
    }

    mt_gotoXX(x1++, y1);            /* Goto last line... */
    assert(bc_printLB(y2) != FAIL); /* Print bottom bound. */

    return SUCCESS;
}

int bc_printbigchar(int a_main[BC_NUM], int x, int y, enum color c_front, enum color c_back)
{
    int term = mt_open();

    /* Copy? Cause can't past in function BY THE VALUE!!! */
    int a[BC_NUM];
    a[BC_FIRST_N] = a_main[BC_FIRST_N];
    a[BC_SECOND_N] = a_main[BC_SECOND_N];

    if (mt_setbgcolor(c_back) == FAIL || mt_setbgcolor(c_front) == FAIL
        || mt_gotoXX(x, y) == FAIL) {
        runtime_error_process(RE.ERROR_ANY_BC);
        return FAIL;
    }

    for (int i = 0; i != BC_NUM; i++) {
        for (int k = 0; k != sizeof(int); ++k) {
            mt_gotoXX(x++, y);
            for (int rad = 0; rad != BIT_S_IN_BYTE; ++rad) {
                int val = a[i] & BIT_ONE;
                if (val == BIT_ONE) {
                    bc_printA(RECT);
                } else {
                    bc_printES(1);
                }
                a[i] >>= SHIFT_MIN;
            }
            bc_printNL();
        }
    }

    close(term);

    return SUCCESS;
}

int bc_setbigcharpos(int* big, int x, int y, int value)
{
    if (big == NULL || x < MIN_LINE_N || x > MAX_LINE_N || y < MIN_LINE_N || y > MAX_LINE_N
        || (value != BIT_FALSE_VALUE && value != BIT_TRUE_VALUE)) {
        runtime_error_process(RE.ERROR_ANY_BC);
        return FAIL;
    }

    if (x < (MAX_LINE_N / 2 + 1)) { /* Char in the 1-4 lines. */
        (value == BIT_ONE)          /* Sets 1 or 0. */
                ? (big[BC_FIRST_N]
                   |= (SHIFT_MIN << ((BIT_S_IN_BYTE * (x - SHIFT_DIFF) + y) - SHIFT_MIN)))
                : (big[BC_FIRST_N]
                   &= (~(SHIFT_MIN << (((BIT_S_IN_BYTE * (x - SHIFT_DIFF) + y)) - SHIFT_MIN))));
    }

    else {                 /* Char in the 5-8 lines. */
        (value == BIT_ONE) /* Sets 1 or 0. */
                ? (big[BC_SECOND_N]
                   |= (SHIFT_MIN << ((BIT_S_IN_BYTE * (x - SHIFT_DIFF) + y) - SHIFT_MIN)))
                : (big[BC_SECOND_N]
                   &= (~(SHIFT_MIN << (((BIT_S_IN_BYTE * (x - SHIFT_DIFF) + y)) - SHIFT_MIN))));
    }

    return SUCCESS;
}

int bc_getbigcharpos(int* big, int x, int y, int* value)
{
    if (big == NULL || x < MIN_LINE_N || x > MAX_LINE_N || y < MIN_LINE_N || y > MAX_LINE_N
        || value == NULL) {
        if (value != NULL) { *value = 0; }
        runtime_error_process(RE.ERROR_ANY_BC);
        return FAIL;
    }

    if (x < MAX_LINE_N / 2 + 1) { /* 1-4 lines. */
        *value = (big[BC_FIRST_N] >> ((BIT_S_IN_BYTE * (x - SHIFT_DIFF) + y) - SHIFT_MIN))
                & BIT_ONE;
    } else { /* 5-8 lines. */
        *value = (big[BC_SECOND_N] >> ((BIT_S_IN_BYTE * (x - SHIFT_DIFF) + y) - SHIFT_MIN))
                & BIT_ONE;
    }

    return SUCCESS;
}

int bc_bigcharwrite(int fd, int* big, int count)
{
    if (big == NULL || count < 1) {
        runtime_error_process(RE.ERROR_ANY_BC);
        return FAIL;
    }

    assert(write(fd, big, sizeof(int) * BC_NUM * count) != FAIL);

    return SUCCESS;
}

int bc_bigcharread(int fd, int* big, int need_count, int* count)
{
    if (big == NULL || need_count < 1 || count == NULL) {
        if (count != NULL) { *count = 0; }
        runtime_error_process(RE.ERROR_ANY_BC);
        return FAIL;
    }

    if (read(fd, big, need_count * sizeof(int) * 2) == FAIL) {
        runtime_error_process(RE.ERROR_ANY_BC);
        return FAIL;
    }

    return SUCCESS;
}
