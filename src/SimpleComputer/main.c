#include <libcomputer/comp.h>

#include <libterm/term.h>

#include <stdio.h>

#include <stdlib.h>

int main()
{
    printf("\033[0m");

    mt_clrscr();

    enum color r = RED;
    enum color y = YELLOW;

    mt_setfgcolor(r);

    printf("RED COLOR???\n");

    int* rows = malloc(sizeof(int));
    int* cols = malloc(sizeof(int));

    mt_setbgcolor(y);

    mt_getscreensize(rows, cols);

    printf("rows - %d\tcols - %d\n", *rows, *cols);

    mt_gotoXX(30, 20);

    int i;
    scanf("%d", &i);
}
