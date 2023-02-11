#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <stdio.h>
#include <stdlib.h>

void print_ext_table()
{
    for (int x = 0; x < 127; ++x) {
        printf("%d - \e(0%c\n\n", x, x);
    }
    printf("\e(B");
}

int main()
{
    // print_ext_table();

    // bc_box(5, 5, 10, 100);

    int arr[2] = {127, 5};

    printf("\e(0%s\e(B", "a");

    bc_printbigchar(arr, 4, 4, BLACK, WHITE);

    // mt_clrscr();
}
