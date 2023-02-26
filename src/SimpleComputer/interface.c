#include <SimpleComputer/interface.h>
#include <common/common.h>
#include <fcntl.h>
#include <libbigchar/bigchar.h>
#include <libcomputer/comp.h>
#include <libterm/term.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

NUM NUMS[]
        = {{.N[0] = 0xFF181818, .N[1] = 0x181818FF},
           {.N[0] = 0xFF000000, .N[1] = 0x000000FF},
           {.N[0] = 0x8181817e, .N[1] = 0x7e818181},
           {.N[0] = 0x8890A0C0, .N[1] = 0x80808080},
           {.N[0] = 0x2040827C, .N[1] = 0xFE040810},
           {.N[0] = 0x6080817E, .N[1] = 0x7E818060},
           {.N[0] = 0xFF818181, .N[1] = 0x80808080},
           {.N[0] = 0x7F0101FF, .N[1] = 0x7F808080},
           {.N[0] = 0x0101817E, .N[1] = 0x7E81817F},
           {.N[0] = 0x204080FE, .N[1] = 0x02040810},
           {.N[0] = 0x7E81817E, .N[1] = 0x7E818181},
           {.N[0] = 0x7E81817E, .N[1] = 0x7E808080}};

// Print RAM in the box beginning in the (x1, y1) coordinate and
// ending in the (x2, y2) coordinate.
int output_memory_in_box(int x1, int y1, int x2, int y2)
{
    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    char buf[BUF_SIZE] = {};

    bc_box(x1, y1, x2, y2);

    int* tval = malloc(sizeof(int));

    // Goto next line...
    ++x1;

    for (size_t i = MIN_MEMORY_ADDRESS; i != MEMORY_SIZE / 10; ++i) {
        // Goto next line...
        if (mt_gotoXX(x1++, y1 + 1) == FAIL) {
            return FAIL;
        }

        // Print row of RAM.
        for (int j = MIN_MEMORY_ADDRESS; j != MEMORY_SIZE / 10; ++j) {
            if (sc_memoryGet(i * (MEMORY_SIZE / 10) + j, tval) == FAIL) {
                return FAIL;
            }

            (*tval & 0x4000) ? printf("-") : printf("+");

            sprintf(buf, "%04X ", *tval);

            write(term, buf, sizeof(buf));
        }

        bc_printNL();
    }

    free(tval);

    mt_gotoXX(1, 31);

    write(term, TITLE_MEMORY, sizeof(TITLE_MEMORY));

    close(term);

    return SUCCESS;
}

int output_accum()
{
    if (bc_box(1, 63, 1, 18) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    mt_gotoXX(1, 67);

    write(term, TITLE_ACCUM, sizeof(TITLE_ACCUM));

    close(term);

    return SUCCESS;
}

int output_instrcounter()
{
    if (bc_box(4, 63, 1, 18) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    mt_gotoXX(4, 64);

    write(term, TITLE_COUNTER, sizeof(TITLE_COUNTER));

    close(term);

    return SUCCESS;
}

int output_operation()
{
    if (bc_box(7, 63, 1, 18) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    mt_gotoXX(7, 68);

    write(term, TITLE_OPERATION, sizeof(TITLE_OPERATION));

    close(term);

    return SUCCESS;
}

int output_flags()
{
    if (bc_box(10, 63, 1, 18) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    mt_gotoXX(10, 70);

    write(term, TITLE_FLAGS, sizeof(TITLE_FLAGS));

    close(term);

    return SUCCESS;
}

int output_keys()
{
    // return bc_box(13, 47, 8, 34);

    if (bc_box(13, 47, 8, 34) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    mt_gotoXX(13, 48);

    write(term, TITLE_KEYS, sizeof(TITLE_KEYS));

    // Writes titles keys.
    mt_gotoXX(14, 48);
    write(term, TITLE_KEYS_LOAD, sizeof(TITLE_KEYS_LOAD));
    mt_gotoXX(15, 48);
    write(term, TITLE_KEYS_SAVE, sizeof(TITLE_KEYS_SAVE));
    mt_gotoXX(16, 48);
    write(term, TITLE_KEYS_RUN, sizeof(TITLE_KEYS_RUN));
    mt_gotoXX(17, 48);
    write(term, TITLE_KEYS_STEP, sizeof(TITLE_KEYS_STEP));
    mt_gotoXX(18, 48);
    write(term, TITLE_KEYS_RESET, sizeof(TITLE_KEYS_RESET));
    mt_gotoXX(19, 48);
    write(term, TITLE_KEYS_ACCUM, sizeof(TITLE_KEYS_ACCUM));
    mt_gotoXX(20, 48);
    write(term, TITLE_KEYS_INSTRCNTER, sizeof(TITLE_KEYS_INSTRCNTER));

    close(term);

    return SUCCESS;
}

int print_mem_cell_sign(NUM a)
{
    return bc_printbigchar(a.N, 14, 2, 10, 44);
}

int print_mem_cell_1(NUM a)
{
    return bc_printbigchar(a.N, 14, 11, 10, 44);
}

int print_mem_cell_2(NUM a)
{
    return bc_printbigchar(a.N, 14, 20, 10, 44);
}

int print_mem_cell_3(NUM a)
{
    return bc_printbigchar(a.N, 14, 29, 10, 44);
}

int print_mem_cell_4(NUM a)
{
    return bc_printbigchar(a.N, 14, 38, 10, 44);
}

int print_MC(int n)
{
    bc_box(13, 1, 8, 44);

    int* num = malloc(sizeof(int));
    sc_memoryGet(n, num);

    if (*num < 0) {
        print_mem_cell_sign(NUMS[1]);
    } else {
        print_mem_cell_sign(NUMS[0]);
    }

    print_mem_cell_4(NUMS[(*num % 10) + 2]);
    *num /= 10;
    print_mem_cell_3(NUMS[(*num % 10) + 2]);
    *num /= 10;
    print_mem_cell_2(NUMS[(*num % 10) + 2]);
    *num /= 10;
    print_mem_cell_1(NUMS[(*num % 10) + 2]);

    return SUCCESS;
}

void bctest()
{
    mt_clrscr();

    {
        int n[2] = {0xFF181818, 0x181818FF};
        bc_setbigcharpos(n, 8, 2, 1);
        bc_printbigchar(n, 5, 5, 100, 44);
    }

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
}

void run_interface()
{
    output_memory_in_box(1, 1, 10, 60);
    output_accum();
    output_instrcounter();
    output_operation();
    output_flags();

    print_MC(10);

    output_keys();

    mt_gotoXX(30, 0);
}
