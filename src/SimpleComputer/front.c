#include <SimpleComputer/front.h>
#include <ctype.h>
#include <fcntl.h>
#include <libbigchar/bigchar.h>
#include <libcommon/common.h>
#include <libcomputer/comp.h>
#include <libreadkey/readkey.h>
#include <libterm/term.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// Big character.
typedef struct NUM {
    int N[2];
} NUM;

static NUM NUMS[]
        = {/* + */ {.N[0] = 0xFF181818, .N[1] = 0x181818FF},
           /* - */ {.N[0] = 0xFF000000, .N[1] = 0x000000FF},
           /* 0 */ {.N[0] = 0x8181817e, .N[1] = 0x7e818181},
           /* 1 */ {.N[0] = 0x8890A0C0, .N[1] = 0x80808080},
           /* 2 */ {.N[0] = 0x2040827C, .N[1] = 0xFE040810},
           /* 3 */ {.N[0] = 0x6080817E, .N[1] = 0x7E818060},
           /* 4 */ {.N[0] = 0xFF818181, .N[1] = 0x80808080},
           /* 5 */ {.N[0] = 0x7F0101FF, .N[1] = 0x7F808080},
           /* 6 */ {.N[0] = 0x0101817E, .N[1] = 0x7E81817F},
           /* 7 */ {.N[0] = 0x204080FE, .N[1] = 0x02040810},
           /* 8 */ {.N[0] = 0x7E81817E, .N[1] = 0x7E818181},
           /* 9 */ {.N[0] = 0x7E81817E, .N[1] = 0x7E808080},
           /* A */ {.N[0] = 0x7E42423C, .N[1] = 0x42424242},
           /* B */ {.N[0] = 0x3E42423E, .N[1] = 0x3E424242},
           /* C */ {.N[0] = 0x0101017E, .N[1] = 0x7E010101},
           /* D */ {.N[0] = 0x4242221E, .N[1] = 0x1E224242},
           /* E */ {.N[0] = 0x7E02027E, .N[1] = 0x7E020202},
           /* F */ {.N[0] = 0x7E02027E, .N[1] = 0x02020202}};

// Computer accumulator.
static __int16_t accumulator = 0;

// Computer instruction counter.
static __uint8_t instruction_counter = 0;

// Actual operation.
static __int16_t actual_operation = 0;

// Actual memory pointer.
// static __uint8_t instruction_counter = 0;

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

    for (__uint8_t i = MEMORY_MIN_ADDRESS; i != MEMORY_SIZE / 10; ++i) {
        // Goto next line...
        if (mt_gotoXX(x1++, y1 + 1) == FAIL) {
            return FAIL;
        }

        // Print row of RAM.
        for (int j = MEMORY_MIN_ADDRESS; j != MEMORY_SIZE / 10; ++j) {
            if (sc_memoryGet(i * (MEMORY_SIZE / 10) + j, tval) == FAIL) {
                return FAIL;
            }

            if (i * (MEMORY_SIZE / 10) + j == instruction_counter) {
                mt_setfgcolor(BLACK);
                mt_setbgcolor(WHITE);
            }

            if (*tval >> 13 == 1) {
                write(term, "-", 2);
                // *tval >>= 1;
            } else {
                write(term, "+", 2);
            }

            sprintf(buf, "%04X", *tval);

            write(term, buf, sizeof(buf));

            if (i * (MEMORY_SIZE / 10) + j == instruction_counter) {
                mt_resetcolor();
            }

            sprintf(buf, " ");

            write(term, buf, 1);
        }

        bc_printNL();
    }

    free(tval);

    mt_gotoXX(1, 30);

    write(term, TITLE_MEMORY, sizeof(TITLE_MEMORY));

    close(term);

    return SUCCESS;
}

int output_accum()
{
    if (bc_box(1, 63, 1, 20) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        return FAIL;
    }

    mt_gotoXX(1, 67);
    write(term, TITLE_ACCUM, sizeof(TITLE_ACCUM));

    int accum_value = accumulator;

    mt_gotoXX(2, 71);

    if ((accum_value & 0x4000) >> 14 == 1) {
        write(term, "-", 2);
        accum_value >>= 1;
    } else {
        write(term, "+", 2);
    }

    char buf[16] = {};
    sprintf(buf, "%04X", accum_value);
    write(term, buf, BUF_SIZE);

    close(term);

    return SUCCESS;
}

int output_instrcounter()
{
    if (bc_box(4, 63, 1, 20) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        return FAIL;
    }

    // Print title.
    mt_gotoXX(4, 64);
    write(term, TITLE_COUNTER, sizeof(TITLE_COUNTER));

    mt_gotoXX(5, 71);

    if ((instruction_counter & 0x4000) >> 14 == 1) {
        write(term, MINUS, sizeof(MINUS));
        instruction_counter >>= 1;
    } else {
        write(term, PLUS, sizeof(PLUS));
    }

    char buf[BUF_SIZE] = {};
    sprintf(buf, "%04X", instruction_counter);
    write(term, buf, sizeof(buf));

    close(term);

    return SUCCESS;
}

int output_operation()
{
    if (bc_box(7, 63, 1, 20) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        return FAIL;
    }

    mt_gotoXX(7, 68);
    write(term, TITLE_OPERATION, sizeof(TITLE_OPERATION));

    char buf[BUF_SIZE] = {};

    int* command = malloc(sizeof(int));
    int* operand = malloc(sizeof(int));

    sc_commandDecode(actual_operation, command, operand);

    sprintf(buf, "%02X : %02X", *command, *operand);

    mt_gotoXX(8, 69);

    if ((actual_operation & 0x4000) >> 14 == 1) {
        write(term, MINUS, sizeof(MINUS));
        instruction_counter >>= 1;
    } else {
        write(term, PLUS, sizeof(PLUS));
    }

    write(term, buf, BUF_SIZE);

    free(command);
    free(operand);

    close(term);

    return SUCCESS;
}

int output_flags()
{
    if (bc_box(10, 63, 1, 20) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
        return FAIL;
    }

    mt_gotoXX(10, 70);
    write(term, TITLE_FLAGS, sizeof(TITLE_FLAGS));

    int* value = malloc(sizeof(int));

    mt_gotoXX(11, 69);
    sc_regGet(FLAG_OVERFLOW_N, value);
    write(term, ((*value) ? FLAG_OVERFLOW : ""), 1);

    mt_gotoXX(11, 71);
    sc_regGet(FLAG_NULL_DIV_N, value);
    write(term, ((*value) ? FLAG_NULL_DIV : ""), 1);

    mt_gotoXX(11, 73);
    sc_regGet(FLAG_UNK_COMMAND_N, value);
    write(term, ((*value) ? FLAG_UNK_COMMAND : ""), 1);

    mt_gotoXX(11, 75);
    sc_regGet(FLAG_IGNORE_N, value);
    write(term, ((*value) ? FLAG_IGNORE : ""), 1);

    mt_gotoXX(11, 77);
    sc_regGet(FLAG_OUT_OF_MEM_N, value);
    write(term, ((*value) ? FLAG_OUT_OF_MEM : ""), 1);

    free(value);

    close(term);

    return SUCCESS;
}

int output_keys()
{
    if (bc_box(13, 47, 8, 36) == FAIL) {
        return FAIL;
    }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) {
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

int output_iofield()
{
    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL) {
        return FAIL;
    }

    mt_gotoXX(23, 0);

    write(term, "Input\\Output: ", sizeof("Input\\Output: "));

    close(term);

    return FAIL;
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

    if (*num >> 13 == 1) {
        print_mem_cell_sign(NUMS[1]);
    } else {
        print_mem_cell_sign(NUMS[0]);
    }

    print_mem_cell_4(NUMS[(*num & 0xf) + 2]);
    print_mem_cell_3(NUMS[((*num >> 4) & 0xf) + 2]);
    print_mem_cell_2(NUMS[((*num >> 8) & 0xf) + 2]);
    print_mem_cell_1(NUMS[((*num >> 12) & 0xf) + 2]);

    free(num);

    return SUCCESS;
}

void reset()
{
    for (size_t i = 0; i != MEMORY_SIZE; ++i) {
        sc_memorySet(i, MEMORY_DEFAULT_VALUE);
    }

    sc_regSet(FLAG_OVERFLOW_N, FLAG_DEFAULT_VALUE);
    sc_regSet(FLAG_NULL_DIV_N, FLAG_DEFAULT_VALUE);
    sc_regSet(FLAG_UNK_COMMAND_N, FLAG_DEFAULT_VALUE);
    sc_regSet(FLAG_IGNORE_N, FLAG_DEFAULT_VALUE);
    sc_regSet(FLAG_OUT_OF_MEM_N, FLAG_DEFAULT_VALUE);
}

void process_key(enum keys* k)
{
    if (*k == RIGHT) {
        instruction_counter += 1;
    } else if (*k == LEFT) {
        instruction_counter -= 1;
    } else if (*k == DOWN) {
        instruction_counter += 10;
    } else if (*k == UP) {
        instruction_counter -= 10;
    } else if (*k == ENTER) {
        int term = mt_open();

        write(0, MSG_INPUT, sizeof(MSG_INPUT));

        setvbuf(stdout, NULL, _IONBF, 0);
        struct termios actual_term_set;
        tcgetattr(0, &actual_term_set);
        actual_term_set.c_lflag |= ISIG;
        actual_term_set.c_lflag |= ICANON;
        tcsetattr(0, TCSANOW, &actual_term_set);

        char buf[10] = {};
        read(0, buf, 10);

        int actual_num = atoi(buf);

        if (actual_num < MEMORY_MAX_CELL_VALUE
            && actual_num >= MEMORY_MIN_CELL_VALUE) {
            sc_memorySet(instruction_counter, actual_num);
        }

        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stdin, NULL, _IONBF, 0);

        close(term);
    } else if (*k == F5) {
        int term = mt_open();

        setvbuf(stdout, NULL, _IONBF, 0);
        struct termios actual_term_set;
        tcgetattr(0, &actual_term_set);
        actual_term_set.c_lflag |= ISIG;
        actual_term_set.c_lflag |= ICANON;
        tcsetattr(0, TCSANOW, &actual_term_set);

        write(0, NEWLINE, sizeof(NEWLINE));
        write(0, MSG_INPUT, sizeof(MSG_INPUT));

        char buf[10] = {};

        read(0, buf, 10);

        __int16_t tmp_accum = atoi(buf);

        if (tmp_accum < MEMORY_MAX_CELL_VALUE
            && tmp_accum >= MEMORY_MIN_CELL_VALUE) {
            accumulator = tmp_accum;
        }

        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stdin, NULL, _IONBF, 0);

        close(term);
    } else if (*k == F6) {
        int term = mt_open();

        setvbuf(stdout, NULL, _IONBF, 0);
        struct termios actual_term_set;
        tcgetattr(0, &actual_term_set);
        actual_term_set.c_lflag |= ISIG;
        actual_term_set.c_lflag |= ICANON;
        tcsetattr(0, TCSANOW, &actual_term_set);

        write(0, NEWLINE, sizeof(NEWLINE));
        write(0, MSG_INPUT, sizeof(MSG_INPUT));

        char buf[10] = {};

        read(0, buf, 10);

        __int16_t new_instr_cnter = atoi(buf);

        if (new_instr_cnter < MEMORY_MAX_ADDRESS
            && new_instr_cnter >= MEMORY_MIN_ADDRESS) {
            instruction_counter = new_instr_cnter;
        }

        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stdin, NULL, _IONBF, 0);

        close(term);
    } else if (*k == RESET) {
        reset();
    } else if (*k == LOAD) {
        int term = mt_open();

        setvbuf(stdout, NULL, _IONBF, 0);
        struct termios actual_term_set;
        tcgetattr(0, &actual_term_set);
        actual_term_set.c_lflag |= ISIG;
        actual_term_set.c_lflag |= ICANON;
        tcsetattr(0, TCSANOW, &actual_term_set);

        write(0, NEWLINE, sizeof(NEWLINE));
        write(0, MSG_INPUT, sizeof(MSG_INPUT));

        char file_name[FILE_NAME_MAX_LEN] = {};

        read(0, file_name, FILE_NAME_MAX_LEN);

        rk_termrestore(file_name);

        // rk_termsave(file_name);

        close(term);
    } else if (*k == SAVE) {
        int term = mt_open();

        setvbuf(stdout, NULL, _IONBF, 0);
        struct termios actual_term_set;
        tcgetattr(0, &actual_term_set);
        actual_term_set.c_lflag |= ISIG;
        actual_term_set.c_lflag |= ICANON;
        tcsetattr(0, TCSANOW, &actual_term_set);

        write(0, NEWLINE, sizeof(NEWLINE));
        write(0, MSG_INPUT, sizeof(MSG_INPUT));

        char file_name[FILE_NAME_MAX_LEN] = {};

        read(0, file_name, FILE_NAME_MAX_LEN);

        rk_termsave(file_name);

        close(term);
    }
}

void interface()
{
    int current_command = 0;

    sc_memoryInit();

    enum keys* k = malloc(sizeof(enum keys));

    while (1) {
        *k = OTHER;

        mt_clrscr();
        output_memory_in_box(1, 1, 10, 60);
        output_accum();
        output_instrcounter(current_command);
        output_operation();
        output_flags();
        output_keys();
        output_iofield();
        print_MC(instruction_counter);

        mt_gotoXX(23, 15);

        rk_readkey(k);
        process_key(k);

        if (instruction_counter > 99) {
            instruction_counter = 0;
        } else if (instruction_counter == 255) {
            instruction_counter = 99;
        }

        int* n = malloc(sizeof(*n)); /* Memory cell number. */
        sc_memoryGet(instruction_counter, n);
        if (*n >> 13 == 0) {
            actual_operation = *n;
            output_operation();
        }
    }

    mt_gotoXX(30, 0);

    sc_memoryDelete();
}
