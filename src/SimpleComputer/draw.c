#include <SimpleComputer/controlunit.h>
#include <SimpleComputer/draw.h>
#include <ctype.h>
#include <fcntl.h>
#include <libbigchar/bigchar.h>
#include <libcommon/common.h>
#include <libcomputer/comp.h>
#include <libreadkey/readkey.h>
#include <libterm/term.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define BUF_SIZE 16

int output_memory_in_box(int x1, int y1, int x2, int y2)
{
    int term = mt_open();
    char buf[BUF_SIZE] = {};
    if (bc_box(x1, y1, x2, y2) == FAIL) {
        close(term);
        return FAIL;
    }

    ++x1; // Goto next line...

    for (__uint8_t i = MEMORY_MIN_ADDRESS; i != MEMORY_SIZE / 10; ++i) {
        if (mt_gotoXX(x1++, y1 + 1) == FAIL) { // Goto next line...
            runtime_error_process(RE.ERROR_TERMINAL_INTERFACE);
            close(term);
            return FAIL;
        }

        for (int j = MEMORY_MIN_ADDRESS; j != MEMORY_SIZE / 10; ++j) { // Print row of RAM.
            __int16_t tval = 0;
            if (sc_memoryGet(i * (MEMORY_SIZE / 10) + j, &tval) == FAIL) {
                runtime_error_process(RE.ERROR_MEM_GET);
                close(term);
                return FAIL;
            }

            if (i * (MEMORY_SIZE / 10) + j
                == instruction_counter) { // Setting color for instruction counter output.
                mt_setfgcolor(BLACK);
                mt_setbgcolor(WHITE);
            }

            write(term, (tval >> 14 == 1) ? "-" : "+", 2); // Output value.

            tval &= 0x7fff;
            __int8_t command = 0, operand = 0;
            sc_commandDecode(tval, &command, &operand);

            sprintf(buf, "%02X%02X", command & 0x7f, operand & 0x7f);
            write(term, buf, sizeof(buf));

            if (i * (MEMORY_SIZE / 10) + j == instruction_counter) { mt_resetcolor(); }

            write(term, " ", 1);
        }

        bc_printNL();
    }

    if (mt_gotoXX(1, 30) == FAIL || write(term, TITLE_MEMORY, sizeof(TITLE_MEMORY)) == FAIL) {
        runtime_error_process(RE.ERROR_TERMINAL_INTERFACE);
        close(term);
        return FAIL;
    }

    close(term);
    return SUCCESS;
}

int output_accum()
{
    int term = mt_open();

    if (bc_box(1, 63, 1, 20) == FAIL) {
        close(term);
        return FAIL;
    }

    mt_gotoXX(1, 67);
    write(term, TITLE_ACCUM, sizeof(TITLE_ACCUM));

    __int16_t accum_value = accumulator;

    mt_gotoXX(2, 71);

    write(term, (accum_value & 0x4000) ? "-" : "+", 2); // Output value.

    if (accum_value < 0) { accum_value = abs(accum_value); }

    char buf[BUF_SIZE] = {};
    sprintf(buf, "%04X", accum_value & 0x3fff);
    write(term, buf, BUF_SIZE);

    close(term);
    return SUCCESS;
}

int output_instrcounter()
{
    if (bc_box(4, 63, 1, 20) == FAIL) { return FAIL; }

    int term = mt_open();

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
    if (bc_box(7, 63, 1, 20) == FAIL) { return FAIL; }

    int term = mt_open();

    mt_gotoXX(7, 68);
    write(term, TITLE_OPERATION, sizeof(TITLE_OPERATION));

    char buf[BUF_SIZE] = {};

    __int8_t command = 0;
    __int8_t operand = 0;

    sc_commandDecode(actual_operation, &command, &operand);

    sprintf(buf, "%02X : %02X", command, operand);

    mt_gotoXX(8, 69);

    write(term, (actual_operation & 0x4000) ? MINUS : PLUS, sizeof(MINUS) - 1);
    write(term, buf, BUF_SIZE);

    close(term);
    return SUCCESS;
}

int output_flags()
{
    if (bc_box(10, 63, 1, 20) == FAIL) { return FAIL; }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) { return FAIL; }

    mt_gotoXX(10, 70);
    write(term, TITLE_FLAGS, sizeof(TITLE_FLAGS));

    __int8_t value;

    mt_gotoXX(11, 69);
    sc_regGet(FLAG_OVERFLOW_N, &value);
    write(term, ((value) ? FLAG_OVERFLOW : ""), 1);

    mt_gotoXX(11, 71);
    sc_regGet(FLAG_NULL_DIV_N, &value);
    write(term, ((value) ? FLAG_NULL_DIV : ""), 1);

    mt_gotoXX(11, 73);
    sc_regGet(FLAG_UNK_COMMAND_N, &value);
    write(term, ((value) ? FLAG_UNK_COMMAND : ""), 1);

    mt_gotoXX(11, 75);
    sc_regGet(FLAG_IGNORE_N, &value);
    write(term, ((value) ? FLAG_IGNORE : ""), 1);

    mt_gotoXX(11, 77);
    sc_regGet(FLAG_OUT_OF_MEM_N, &value);
    write(term, ((value) ? FLAG_OUT_OF_MEM : ""), 1);

    close(term);
    return SUCCESS;
}

int output_keys()
{
    if (bc_box(13, 47, 8, 36) == FAIL) { return FAIL; }

    // Turn off buffering.
    setvbuf(stdout, NULL, _IONBF, 0);

    int term = open(TERM_PATH, O_WRONLY);

    if (term == FAIL || isatty(term) == 0) { return FAIL; }

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
    if (term == FAIL) { return FAIL; }

    mt_gotoXX(24, 2);

    write(term, "> ", sizeof("> "));
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

    __int16_t num = 0;
    sc_memoryGet(n, &num);

    if (num >> 14 == 0) {
        print_mem_cell_sign(NUMS[0]);
    } else {
        print_mem_cell_sign(NUMS[1]);
    }

    num &= 0x3fff;

    __int8_t command = 0, operand = 0;

    sc_commandDecode(num, &command, &operand);

    print_mem_cell_4(NUMS[(operand & 0xf) + 2]);
    print_mem_cell_3(NUMS[((operand >> 4) & 0xf) + 2]);
    print_mem_cell_2(NUMS[(command & 0xf) + 2]);
    print_mem_cell_1(NUMS[((command >> 4) & 0xf) + 2]);

    return SUCCESS;
}

void print_output_box()
{
    int term = mt_open();
    bc_box(23, 47, 5, 36);
    mt_gotoXX(23, 62);
    write(term, TITLE_OUTPUT, sizeof(TITLE_OUTPUT));
    close(term);
}

void print_input_box()
{
    int term = mt_open();
    bc_box(23, 0, 5, 44);
    mt_gotoXX(23, 20);
    write(term, TITLE_INPUT, sizeof(TITLE_INPUT));
    close(term);
}

void GUI()
{
    output_memory_in_box(1, 1, 10, 60);
    output_accum();
    output_instrcounter(current_command);
    output_operation();
    output_flags();
    output_keys();
    print_output_box();
    print_input_box();
    print_MC(instruction_counter);
    output_iofield();
}
