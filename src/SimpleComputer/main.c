#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <libbigchar/bigchar.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("EXIT_CODE - %d\n", bc_box(2, 2, 3, 3));

	return 0;
}
