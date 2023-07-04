#include <SimpleComputer/ALU.h>
#include <SimpleComputer/controlunit.h>
#include <SimpleComputer/draw.h>
#include <libbigchar/bigchar.h>
#include <libcommon/common.h>
#include <libcomputer/comp.h>
#include <libreadkey/readkey.h>
#include <libterm/term.h>

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

__int16_t last_output = 0;

__int16_t commands[] = {
        0x10, /* READ */
        0x11, /* WRITE */
        0x20, /* LOAD */
        0x21, /* STORE */
        0x40, /* JUMP */
        0x41, /* JNEG */
        0x42, /* JZ */
        0x43  /* HALT */
};

NUM NUMS[]
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

/* Computer accumulator. */
__int16_t accumulator = 0;

/* Computer instruction counter. */
__uint8_t instruction_counter = 0;

/* Actual operation. */
__int16_t actual_operation = 0;

/* Actual operation. */
__int16_t current_command = 0;

char* itoa(int val, int base)
{
    static char buf[32] = {0};
    int i = 30;
    for (; val && i; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i + 1];
}

void CU_icnt_increaser(__attribute__((unused)) int signal)
{
    ++instruction_counter;
}

void CU_reset(__attribute__((unused)) int signal)
{
    for (size_t i = 0; i != MEMORY_SIZE; ++i) {
        sc_memorySet(i, MEMORY_DEFAULT_VALUE);
    }

    sc_regSet(FLAG_OVERFLOW_N, FLAG_DEFAULT_VALUE);
    sc_regSet(FLAG_NULL_DIV_N, FLAG_DEFAULT_VALUE);
    sc_regSet(FLAG_UNK_COMMAND_N, FLAG_DEFAULT_VALUE);
    sc_regSet(FLAG_IGNORE_N, FLAG_DEFAULT_VALUE);
    sc_regSet(FLAG_OUT_OF_MEM_N, FLAG_DEFAULT_VALUE);

    accumulator = 0;
    instruction_counter = 0;
}

void CU_clean_n_print_gui()
{
    mt_clrscr();
    GUI();
    mt_gotoXX(26, 64);
    printf("%d", last_output);
    mt_gotoXX(24, 4);
}

void CU_check_n_reset_comp()
{
    if (instruction_counter > MEMORY_MAX_ADDRESS) { /* Reset all computer. */
        raise(SIGUSR1);
        sc_regSet(FLAG_IGNORE_N, BIT_ONE);
    }
}

void CU_process_key_ENTER()
{
    char buf[255] = {};
    read(0, buf, 255);

    int minus_flag = 0;
    __int16_t actual_num = 0;

    if (buf[0] == '-') {
        minus_flag = 1;
        buf[0] = '0';
        actual_num = strtol(buf, NULL, 16);
    } else {
        actual_num = strtol(buf, NULL, 16);
    }

    if (minus_flag == 1) { actual_num |= 0x4000; }

    sc_memorySet(instruction_counter, actual_num);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

void CU_process_key_F5()
{
    char buf[255] = {};
    read(0, buf, 255);

    int minus = 0;
    __int16_t tmp_accum = 0;

    if (buf[0] == '-') {
        buf[0] = 0;
        minus = 1;
        tmp_accum = strtol(buf + 1, NULL, 16);
    } else {
        tmp_accum = strtol(buf, NULL, 16);
    }

    if (tmp_accum < MEMORY_MAX_CELL_VALUE && tmp_accum >= MEMORY_MIN_CELL_VALUE) {
        accumulator = tmp_accum;
        if (minus == 1) { accumulator |= 0x4000; }
    }
}

void CU_process_key_F6()
{
    char buf[255] = {};
    read(0, buf, 255);
    __int16_t new_instr_cnter = (__int16_t)atoi(buf);
    if (new_instr_cnter < MEMORY_MAX_ADDRESS && new_instr_cnter >= MEMORY_MIN_ADDRESS) {
        instruction_counter = new_instr_cnter;
    }
}

void CU_process_key_LOAD()
{
    char buf[255] = {};
    read(0, buf, 255);
    buf[strlen(buf) - 1] = '\0';
    sc_memoryLoad(buf);
}

void CU_process_key_SAVE()
{
    char buf[255] = {};
    read(0, buf, 255);
    buf[strlen(buf) - 1] = '\0';
    sc_memorySave(buf);
}

void CU_process_key(enum keys* k)
{
    if (*k == RESET) {
        raise(SIGUSR1);
        sc_regSet(FLAG_IGNORE_N, BIT_ONE);
    } else if (*k == RUN) {
        sc_regSet(FLAG_IGNORE_N, BIT_ZERO);
    } else if (*k == STEP) {
        __int8_t command = 0, operand = 0;
        __int16_t mem_val = 0;
        sc_memoryGet(instruction_counter, &mem_val);
        if (mem_val >> 14 == BIT_ZERO) {
            sc_commandDecode(actual_operation, &command, &operand);
            ALU(command, operand);
            raise(SIGALRM);
        }
    }

    if (*k == ENTER || *k == F5 || *k == F6 || *k == LOAD || *k == SAVE) {
        int term = mt_open();

        write(0, MSG_INPUT, sizeof(MSG_INPUT));

        setvbuf(stdout, NULL, _IONBF, 0);
        struct termios actual_term_set;
        tcgetattr(0, &actual_term_set);
        actual_term_set.c_lflag |= ISIG;
        actual_term_set.c_lflag |= ICANON;
        tcsetattr(0, TCSANOW, &actual_term_set);

        if (*k == ENTER)
            CU_process_key_ENTER();
        else if (*k == F5)
            CU_process_key_F5();
        else if (*k == F6)
            CU_process_key_F6();
        else if (*k == LOAD)
            CU_process_key_LOAD();
        else if (*k == SAVE)
            CU_process_key_SAVE();

        close(term);
    }
}

void CU_correct_actual_oper(__int16_t n)
{
    n = 0;
    sc_memoryGet(instruction_counter, &n);
    if (n >> 14 == 0) { /* Command. */
        actual_operation = n;
        output_operation();
        sc_regSet(FLAG_UNK_COMMAND_N, BIT_ZERO);
    } else { /* Not a command. */
        sc_regSet(FLAG_UNK_COMMAND_N, BIT_ONE);
        sc_regSet(FLAG_IGNORE_N, BIT_ONE);
    }
}

void CU_detect_n_execute_program(
        __int8_t* ignore_state,
        __attribute__((unused)) struct itimerval* nval,
        __attribute__((unused)) struct itimerval* oval,
        enum keys* k)
{
    sc_regGet(FLAG_IGNORE_N, ignore_state);
    if (*ignore_state == BIT_ZERO) {
        setitimer(ITIMER_REAL, nval, oval); /* Start timer. */
        pause();
        __int8_t operand = 0, command = 0;
        sc_commandDecode(actual_operation, &command, &operand);
        if (ALU(command, operand) == FAIL) {
            if (command == 0x10) { /* READ */
                int term = mt_open();

                write(0, MSG_INPUT, sizeof(MSG_INPUT));

                setvbuf(stdout, NULL, _IONBF, 0);
                struct termios actual_term_set;
                tcgetattr(0, &actual_term_set);
                actual_term_set.c_lflag |= ISIG;
                actual_term_set.c_lflag |= ICANON;
                tcsetattr(0, TCSANOW, &actual_term_set);

                char buf[10] = {};

                alarm(0);
                read(0, buf, 10);

                int minus_flag = 0;
                __int16_t actual_num = 0;

                if (buf[0] == '-') {
                    minus_flag = 1;
                    buf[0] = '0';
                    actual_num = strtol(buf + 1, NULL, 16);
                } else {
                    actual_num = strtol(buf, NULL, 16);
                }

                if (minus_flag == 1) { actual_num |= 0x4000; }

                if (actual_num < 0x7fff) { sc_memorySet(operand, actual_num); }
                setvbuf(stdout, NULL, _IONBF, 0);
                setvbuf(stdin, NULL, _IONBF, 0);

                close(term);

                setitimer(ITIMER_REAL, nval, oval);
            } else if (command == 0x11) { /* WRITE */
                __int16_t value = -1;
                sc_memoryGet(operand, &value);
                if (value != -1) { last_output = value; }
                char* buf = itoa(value, 16);
                int term = mt_open();
                mt_gotoXX(26, 62);
                write(term, buf, 4);
                if (strlen(buf) == 0) { write(term, "0", 2); }
            } else if (command == 0x20) /* LOAD */
            {
                __int16_t value = 0;
                sc_memoryGet(operand, &value);
                accumulator = value;
            } else if (command == 0x21) /* STORE */ {
                sc_memorySet(operand, accumulator);
            } else if (command == 0x40) /* JUMP */ {
                instruction_counter = operand;
            } else if (command == 0x41) /* JNEG */ {
                if (accumulator & 0x4000) { instruction_counter = operand; }
            } else if (command == 0x42) /* JZ */ {
                if (accumulator == 0) { instruction_counter = operand; }
            } else if (command == 0x43) /* HALT */ {
                sc_regSet(FLAG_IGNORE_N, BIT_ONE);
            }
        }
    } else {
        alarm(0);
        rk_readkey(k);
        CU_process_key(k);
    }
}

void interface()
{
    sc_memoryInit();                   /* Memory initialization. */
    sc_regInit();                      /* Registers initialization. */
    sc_regSet(FLAG_IGNORE_N, BIT_ONE); /* Set register to ignore commands. */
    __int16_t oper_detecter = 0;       /* Memory cell number. */
    __int8_t ignore_state = BIT_ZERO;  /* Ignoring ticks state. */
    enum keys k;                       /* Processing input keys. */
    struct itimerval nval = {.it_interval.tv_sec = 1,
                             .it_interval.tv_usec = 0,
                             .it_value.tv_sec = 0,
                             .it_value.tv_usec = 80000},
                     oval = {.it_interval.tv_sec = 0,
                             .it_interval.tv_usec = 0,
                             .it_value.tv_sec = 0,
                             .it_value.tv_usec = 0}; /* Timers. */
    signal(SIGALRM, CU_icnt_increaser);              /* Set signal state from alarm. */
    signal(SIGUSR1, CU_reset);                       /* Reset computer. */

    while (1) {
        /* Reset key. */
        k = OTHER;
        /* Reset computer if instrcnter is OOR. */
        CU_check_n_reset_comp();
        /* Print gui. */
        CU_clean_n_print_gui();
        /* Read key or set timer & execute. */
        CU_detect_n_execute_program(&ignore_state, &nval, &oval, &k);
        /* Check and set actual operations. */
        CU_correct_actual_oper(oper_detecter);
    }

    mt_gotoXX(30, 0);
    sc_memoryDelete();
}