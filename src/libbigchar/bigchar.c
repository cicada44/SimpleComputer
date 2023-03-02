#include <common/common.h>
#include <fcntl.h>
#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Extended ascii chars.
#define RECT "a"
#define HR "q"
#define VT_BOUND "x"
#define RB "j"
#define RU "k"
#define LU "l"
#define LB "m"

// 1st and 2nd ints in bigchar array.
#define BC_FIRST_N 0
#define BC_SECOND_N 1

// Min line number in bigchar array.
#define MIN_LINE_N 1
// Max line number in bigchar array.
#define MAX_LINE_N 8

// Prints newline.
int bc_printNL() {
  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL) {
    return FAIL;
  }

  write(term, NEWLINE, strlen(NEWLINE));

  close(term);

  return SUCCESS;
}

int bc_printA(char *str) {
  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL) {
    return FAIL;
  }

  char buf[strlen(str)];

  sprintf(buf, "\e(0%s\e(B", str);

  write(term, buf, strlen("\e(0%s\e(B"));

  close(term);

  return SUCCESS;
}

int bc_printUB(int len) {
  if (bc_printA(LU) == FAIL) {
    return FAIL;
  }

  for (int i = 0; i != len; ++i) {
    if (bc_printA(HR) == FAIL) {
      return FAIL;
    }
  }

  if (bc_printA(RU) == FAIL) {
    return FAIL;
  }

  bc_printNL();

  return SUCCESS;
}

int bc_printLB(int len) {
  if (bc_printA(LB) == FAIL) {
    return FAIL;
  }

  for (int i = 0; i != len; ++i) {
    if (bc_printA(HR) == FAIL) {
      return FAIL;
    }
  }
  if (bc_printA(RB) == FAIL) {
    return FAIL;
  }

  bc_printNL();

  return SUCCESS;
}

int bc_printES(int len) {
  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL) {
    return FAIL;
  }

  for (int i = 0; i != len; ++i) {
    write(term, " ", 2);
  }

  close(term);

  return SUCCESS;
}

int bc_box(int x1, int y1, int x2, int y2) {
  // Turn off buffering.
  setvbuf(stdout, NULL, _IONBF, 0);

  if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
    return FAIL;
  }

  if (mt_gotoXX(x1, y1) == FAIL) {
    return FAIL;
  }

  // Print upper bound.
  if (bc_printUB(y2) == FAIL) {
    return FAIL;
  }

  // Goto next line...
  ++x1;

  for (int i = 0; i != x2; ++i) {
    mt_gotoXX(x1++, y1);

    if (bc_printA(VT_BOUND) == FAIL) {
      return FAIL;
    }

    bc_printES(y2);

    if (bc_printA(VT_BOUND) == FAIL) {
      return FAIL;
    }

    bc_printNL();
  }

  // Goto last line...
  mt_gotoXX(x1++, y1);

  // Print bottom bound.
  if (bc_printLB(y2) == FAIL) {
    return FAIL;
  }

  return SUCCESS;
}

int bc_printbigchar(int a_main[BC_NUM], int x, int y, enum color c_front,
                    enum color c_back) {
  // Copy? Cause can't past in function BY THE VALUE!!!
  int a[BC_NUM];
  a[BC_FIRST_N] = a_main[BC_FIRST_N];
  a[BC_SECOND_N] = a_main[BC_SECOND_N];

  int term = open(TERM_PATH, O_WRONLY);

  if (term == FAIL || isatty(term) == 0) {
    return FAIL;
  }

  if (mt_setbgcolor(c_back) == FAIL) {
    return FAIL;
  }

  if (mt_setbgcolor(c_front) == FAIL) {
    return FAIL;
  }

  if (mt_gotoXX(x, y) == FAIL) {
    return FAIL;
  }

  for (int i = 0; i != BC_NUM; i++) {
    for (int k = 0; k != sizeof(int); ++k) {
      mt_gotoXX(x++, y);
      for (int rad = 0; rad != BITS_IN_BYTE; ++rad) {
        int val = a[i] & ONE_BIT;
        if (val == ONE_BIT) {
          bc_printA(RECT);
        } else {
          bc_printES(1);
        }
        a[i] >>= MIN_SHIFT;
      }
      bc_printNL();
    }
  }

  close(term);

  return SUCCESS;
}

int bc_setbigcharpos(int *big, int x, int y, int value) {
  if (big == NULL || x < MIN_LINE_N || x > MAX_LINE_N || y < MIN_LINE_N ||
      y > MAX_LINE_N || (value != FALSE_BIT_VALUE && value != TRUE_BIT_VALUE)) {
    return FAIL;
  }

  // Char in the 1-4 lines.
  if (x < (MAX_LINE_N / 2 + 1)) {
    // Sets 1 or 0.
    (value == ONE_BIT)
        ? (big[BC_FIRST_N] |=
           (MIN_SHIFT << ((BITS_IN_BYTE * (x - SHIFT_DIFF) + y) - MIN_SHIFT)))
        : (big[BC_FIRST_N] &=
           (~(MIN_SHIFT << (((BITS_IN_BYTE * (x - SHIFT_DIFF) + y)) -
                            MIN_SHIFT))));
  }
  // Char in the 5-8 lines.
  else {
    // Sets 1 or 0.
    (value == ONE_BIT)
        ? (big[BC_SECOND_N] |=
           (MIN_SHIFT << ((BITS_IN_BYTE * (x - SHIFT_DIFF) + y) - MIN_SHIFT)))
        : (big[BC_SECOND_N] &=
           (~(MIN_SHIFT << (((BITS_IN_BYTE * (x - SHIFT_DIFF) + y)) -
                            MIN_SHIFT))));
  }

  return SUCCESS;
}

int bc_getbigcharpos(int *big, int x, int y, int *value) {
  if (big == NULL || x < MIN_LINE_N || x > MAX_LINE_N || y < MIN_LINE_N ||
      y > MAX_LINE_N || value == NULL) {
    if (value != NULL) {
      *value = 0;
    }
    return FAIL;
  }

  if (x < MAX_LINE_N / 2 + 1) {
    *value = (big[BC_FIRST_N] >>
              ((BITS_IN_BYTE * (x - SHIFT_DIFF) + y) - MIN_SHIFT)) &
             ONE_BIT;
  } else {
    *value = (big[BC_SECOND_N] >>
              ((BITS_IN_BYTE * (x - SHIFT_DIFF) + y) - MIN_SHIFT)) &
             ONE_BIT;
  }

  return SUCCESS;
}

int bc_bigcharwrite(int fd, int *big, int count) {
  if (big == NULL || count < 1) {
    return FAIL;
  }

  if (write(fd, big, sizeof(int) * BC_NUM * count) == -1) {
    return FAIL;
  }

  return SUCCESS;
}

int bc_bigcharread(int fd, int *big, int need_count, int *count) {
  if (big == NULL || need_count < 1 || count == NULL) {
    if (count != NULL) {
      *count = 0;
    }
    return FAIL;
  }

  if (read(fd, big, need_count * sizeof(int) * 2) == FAIL) {
    return FAIL;
  }

  return SUCCESS;
}
