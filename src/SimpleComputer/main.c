#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("EXIT_CODE - %d\n", bc_box(2, 2, 3, 3));

    printf("%d", 0x59);

    return 0;
}
