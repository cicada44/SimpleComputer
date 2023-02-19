#include <SimpleComputer/interface.h>
#include <common/common.h>
#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NUMBERS NUMS
        = {{0xFF181818, 0x181818FF},
           {0xFF000000, 0x000000FF},
           {0x8181817e, 0x7e818181},
           {0x8890A0C0, 0x80808080},
           {0x2040827C, 0xFE040810},
           {0x6080817E, 0x7E818060},
           {0xFF818181, 0x80808080},
           {0x7F0101FF, 0x7F808080},
           {0x0101817E, 0x7E81817F},
           {0x204080FE, 0x02040810},
           {0x7E81817E, 0x7E818181},
           {0x7E81817E, 0x7E808080}};

int output_memory_in_box(int x1, int y1, int x2, int y2)
{
    bc_box(x1, y1, x2, y2);

    int* tval = malloc(sizeof(int));

    ++x1;

    for (size_t i = MIN_MEMORY_ADDRESS; i != 10; ++i) {
        if (mt_gotoXX(x1++, y1 + 1) == FAIL_CODE) {
            return FAIL_CODE;
        }

        for (int j = MIN_MEMORY_ADDRESS; j != 10; ++j) {
            if (sc_memoryGet(i * 10 + j, tval) == FAIL_CODE) {
                return FAIL_CODE;
            }

            (*tval & 0x4000) ? printf("-") : printf("+");
            printf("%04X ", *tval);
        }

        printf(NEWLINE);
    }

    free(tval);

    return SUCCESS_CODE;
}

int output_accum()
{
    return bc_box(
            BEGIN_COORD_X,
            END_COORD_Y + 3,
            BEGIN_COORD_X,
            BEGIN_COORD_Y + DEFAULT_FIELDS_LENGHT);
}

int output_instrcounter()
{
    return bc_box(
            BEGIN_COORD_X + DEFAULT_FIELDS_SHIFT,
            END_COORD_Y + 3,
            BEGIN_COORD_X,
            BEGIN_COORD_Y + DEFAULT_FIELDS_LENGHT);
}

int output_operation()
{
    return bc_box(
            BEGIN_COORD_X + DEFAULT_FIELDS_SHIFT * 2,
            END_COORD_Y + 3,
            BEGIN_COORD_X,
            BEGIN_COORD_Y + DEFAULT_FIELDS_LENGHT);
}

int output_flags()
{
    return bc_box(
            BEGIN_COORD_X + DEFAULT_FIELDS_SHIFT * 3,
            END_COORD_Y + 3,
            BEGIN_COORD_X,
            BEGIN_COORD_Y + DEFAULT_FIELDS_LENGHT);
}

int output_keys()
{
    return bc_box(END_COORD_X + 3, 47, END_COORD_X - 2, 34);
}

int output_memory_cell(int sign[2], int n1[2], int n2[2], int n3[2], int n4[2])
{
    bc_box(END_COORD_X + 3, BEGIN_COORD_Y, END_COORD_X - 2, 44);

    bc_printbigchar(sign, END_COORD_X + 4, BEGIN_COORD_Y + 1, END_COORD_X, 44);
    bc_printbigchar(
            n1,
            END_COORD_X + 4,
            BEGIN_COORD_Y + MEMORY_CELL_NUMBER_SHIFT + 1,
            10,
            44);
    bc_printbigchar(
            n2,
            END_COORD_X + 4,
            BEGIN_COORD_Y + MEMORY_CELL_NUMBER_SHIFT * 2 + 1,
            10,
            44);
    bc_printbigchar(
            n3,
            END_COORD_X + 4,
            BEGIN_COORD_Y + MEMORY_CELL_NUMBER_SHIFT * 3 + 1,
            10,
            44);
    bc_printbigchar(
            n4,
            END_COORD_X + 4,
            BEGIN_COORD_Y + MEMORY_CELL_NUMBER_SHIFT * 4 + 1,
            10,
            44);

    return SUCCESS_CODE;
}

void output_computer()
{
    mt_clrscr();

    // {
    // int n[2] = {0xFF181818, 0x181818FF};
    // bc_setbigcharpos(n, 8, 2, 1);
    // bc_printbigchar(n, 5, 5, 100, 44);
    // }

    {
        int* val = malloc(sizeof(int));
        int n[2] = {0xFF181818, 0x181818FF};
        bc_setbigcharpos(n, 8, 2, 1);
        // bc_printbigchar(n, 5, 5, 100, 44);

        for (int x = 1; x < 9; ++x) {
            for (int j = 1; j < 9; ++j) {
                bc_getbigcharpos(n, x, j, val);
                printf("%d ", *val);
            }
            printf(NEWLINE);
        }

        // printf("VAL - %d\n", *val);
    }

    // output_memory_in_box(
    // BEGIN_COORD_X, BEGIN_COORD_Y, END_COORD_X, END_COORD_Y);
    // output_accum();
    // output_instrcounter();
    // output_operation();
    // output_flags();
    // output_memory_cell(NUMS.ZERO, NUMS.FOUR, NUMS.FIVE, NUMS.EIGHT,
    // NUMS.NINE); output_keys();

    mt_gotoXX(END_COORD_X + MEMORY_CELL_LEN, 0);
}
