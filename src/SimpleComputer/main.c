#include <SimpleComputer/front.h>
#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <stdio.h>

void bc_test()
{
    mt_clrscr();
    bc_setbigcharpos(NUMS[0].N, 1, 1, 1);
    bc_printbigchar(NUMS[0].N, 5, 5, 38, 10);
}

int main(void)
{
    // bc_test();

    interface();
}
