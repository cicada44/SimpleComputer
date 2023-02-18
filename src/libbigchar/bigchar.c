#include <common/common.h>
#include <fcntl.h>
#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define EXT_SYM(x) "\e(0%s\e(B", x

#define RECT "a"
#define HR "q"
#define VT_BOUND "x"
#define RB "j"
#define RU "k"
#define LU "l"
#define LB "m"

int bc_printA(char *str) {
  printf("\e(0%s\e(B", str);

  return SUCCESS_CODE;
}

int bc_box(int x1, int y1, int x2, int y2) {
  if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
    return FAIL_CODE;
  }

  if (mt_gotoXX(x1, y1) == FAIL_CODE) {
    return FAIL_CODE;
  }

  // UPPER BOUND
  bc_printA(LU);
  for (int x = 0; x != y2; ++x) {
    if (bc_printA(HR) == FAIL_CODE) {
      return FAIL_CODE;
    }
  }

  bc_printA(RU);
  printf(NEWLINE);

  ++x1;

  for (int i = 0; i != x2; ++i) {
    mt_gotoXX(x1++, y1);

    if (bc_printA(VT_BOUND) == FAIL_CODE) {
      return FAIL_CODE;
    }

    for (int j = 0; j != y2; ++j) {
      printf(" ");
    }

    if (bc_printA(VT_BOUND) == FAIL_CODE) {
      return FAIL_CODE;
    }

    printf(NEWLINE);
  }

  mt_gotoXX(x1++, y1);

  // BOTTOM BOUND
  bc_printA(LB);
  for (int x = 0; x != y2; ++x) {
    if (bc_printA(HR) == FAIL_CODE) {
      return FAIL_CODE;
    }
  }
  bc_printA(RB);

  printf(NEWLINE);

  return SUCCESS_CODE;
}

int bc_printbigchar(int a[2], int x, int y, enum color c_front,
                    enum color c_back) {
  int term = open(TERM_PATH, O_WRONLY);

  if (term == -1) {
    return FAIL_CODE;
  }

  if (mt_setbgcolor(c_back) == FAIL_CODE) {
    return FAIL_CODE;
  }

  if (mt_setbgcolor(c_front) == FAIL_CODE) {
    return FAIL_CODE;
  }

  if (mt_gotoXX(x, y) == FAIL_CODE) {
    return FAIL_CODE;
  }

  for (int i = 0; i != 2; i++) {
    for (int k = 0; k != 4; ++k) {
      mt_gotoXX(x++, y);
      for (int rad = 0; rad != 8; ++rad) {
        int val = a[i] & ONE_BIT;
        if (val == 1) {
          bc_printA(RECT);
        } else {
          printf(" ");
        }
        a[i] >>= 1;
      }
      printf(NEWLINE);
    }
  }

  return SUCCESS_CODE;
}
