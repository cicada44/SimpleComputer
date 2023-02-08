#include <libcomputer/comp.h>

#include <libterm/term.h>

#include <stdio.h>

#include <stdlib.h>

int main()
{
    // mt_clrscr();

    enum colors r = RED;
    enum colors y = YELLOW;

    mt_setfgcolor(r);

    printf("RED COLOR???\n");

    int* rows = malloc(sizeof(int));
    int* cols = malloc(sizeof(int));

    mt_setfgcolor(y);

    mt_getscreensize(rows, cols);

    printf("rows - %d\tcols - %d\n", *rows, *cols);
}
