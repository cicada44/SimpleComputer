#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    mt_clrscr();

    mt_gotoXX(5, 5);

    mt_setbgcolor(GREEN);
    mt_setfgcolor(BLUE);

    printf("AAAAAAAA\n");

    mt_resetcolor();

    return 0;
}
