#pragma once

#include <libreadkey/readkey.h>
#include <stdio.h>
#include <sys/time.h>

#define MSG_INPUT "> "

typedef struct NUM {
    int N[2];
} NUM; /* Big character. */

extern NUM NUMS[];

char* itoa(int val, int base);

/* Instruction counter. */
extern __uint8_t instruction_counter;

/* Actual operation. */
extern __int16_t actual_operation;

/* Computer accumulator. */
extern __int16_t accumulator;

/* Current executable command. */
extern __int16_t current_command;

/* Increases instruction counter. */
void CU_icnt_increaser(__attribute__((unused)) int signal);

/* Main part of control unit. */
void interface();

/* Reset state of all computer. */
void CU_reset(__attribute__((unused)) int signal);

/* Processes the entered character. */
void CU_process_key(enum keys* k);

/* Clears terminal & print gui interface.  */
void CU_clean_n_print_gui();

/* Corrects actual operation counter. */
void CU_correct_actual_oper(__int16_t n);

/* Checks the instruction counter position and reset all computer if
 * it > MAX_MEMORY_ADDRESS */
void CU_check_n_reset_comp();

/* Detects next event. If ignore state == 0 sets timer & executes ALU. Otherwise
 * readkey.*/
void CU_detect_n_execute_program(
        __int8_t* ignore_state,
        struct itimerval* nval,
        struct itimerval* oval,
        enum keys* k);
