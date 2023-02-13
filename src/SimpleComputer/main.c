#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void trytocatchme() {
  mt_setbgcolor(BLACK);
  mt_setfgcolor(YELLOW);

  srand(time(0));

  int *r = malloc(sizeof(int));
  int *c = malloc(sizeof(int));

  while (1) {
    enum color cf = 30 + rand() % 8;
    mt_setbgcolor(cf);

    enum color cb = 30 + rand() % 8;
    mt_setbgcolor(cb);

    mt_getscreensize(r, c);

    // mt_gotoXX(1, 1);
    printf("CATСHMEEEEEEEEEEEEE\n");

    mt_gotoXX(rand() % *r, rand() % *c);
    printf("r - %d, c - %d\n", *r, *c);

    sleep(1);

    mt_clrscr();
  }
}

int main() {
  // trytocatchme();

  bc_box(5, 5, 40, 40);

  // mt_clrscr();
}
