#pragma once

/* Processes errors output. */
void runtime_error_process(const char* const error_msg);

/* Standard error length. */
#define ERROR_LEN 50

/*  Error messages. */

struct RUNTIME_ERRORS {
  char ERROR_OPENING_TERM[ERROR_LEN];
  char ERROR_OPENING_FILE[ERROR_LEN];
  char ERROR_FILE_READING[ERROR_LEN];
  char ERROR_FILE_WRITING[ERROR_LEN];
  char ERROR_TERM_OPT[ERROR_LEN];
  char ERROR_MEM_INIT[ERROR_LEN];
  char ERROR_MEM_GET[ERROR_LEN];
  char ERROR_MEM_SET[ERROR_LEN];
  char ERROR_REG_GET[ERROR_LEN];
  char ERROR_REG_SET[ERROR_LEN];
  char ERROR_COMMAND_ENCODE[ERROR_LEN];
  char ERROR_COMMAND_DECODE[ERROR_LEN];
  char ERROR_ANY_TERM[ERROR_LEN];
  char ERROR_ANY_BC[ERROR_LEN];
  char ERROR_READING_KEY[ERROR_LEN];
  char ERROR_TERMINAL_INTERFACE[ERROR_LEN];
};

extern struct RUNTIME_ERRORS RE;

/* Functions return value. */

#define FAIL -1
#define SUCCESS 0

/* Newline string literal. */
#define NEWLINE "\n"

/* Registrers. */

#define MIN_REGISTER_NUM 1
#define MAX_REGISTER_NUM 5

/* Bits. */

#define BIT_TRUE_VALUE 1
#define BIT_FALSE_VALUE 0
#define BIT_S_IN_BYTE 8
#define BIT_ZERO 0x0
#define BIT_ONE 0x1

/* Shifts. */

#define SHIFT_MIN 1
#define SHIFT_DECODE 7
#define SHIFT_DIFF 1

/* Memory. */

#define MEMORY_SIZE 100
#define MEMORY_MIN_ADDRESS 0
#define MEMORY_MAX_ADDRESS 99
#define MEMORY_DEFAULT_VALUE 0
#define MEMORY_MAX_CELL_VALUE 0x7fff
#define MEMORY_MIN_CELL_VALUE 0x0

/* Titles. */

#define TITLE_MEMORY " Memory "
#define TITLE_ACCUM " accumulator "
#define TITLE_COUNTER " instructionCounter "
#define TITLE_OPERATION " Operation "
#define TITLE_FLAGS " Flags "
#define TITLE_KEYS " Keys: "
#define TITLE_KEYS_LOAD "l  - load"
#define TITLE_KEYS_SAVE "s  - save"
#define TITLE_KEYS_RUN "r  - run"
#define TITLE_KEYS_STEP "t  - step"
#define TITLE_KEYS_RESET "i  - reset"
#define TITLE_KEYS_ACCUM "F5 - accumulator"
#define TITLE_KEYS_INSTRCNTER "F6 - instructionCounter"

/* Maximal command code. */
#define MAX_COMMAND_OPERATOR_VALUE 0x7f

/* Default output buffer size. */
#define BUF_SIZE 15

/* Default terminal stats. */

#define TERM_PATH "/dev/tty"
#define TERMINAL_OPENCODE 1

/* Minimal beginning coordinate. */
#define MIN_BEGIN_COORD 0

/* Number of ints in bigchar. */
#define BC_NUM 2

/* Register flags names. */

#define FLAG_OVERFLOW "O"
#define FLAG_NULL_DIV "N"
#define FLAG_UNK_COMMAND "E"
#define FLAG_IGNORE "V"
#define FLAG_OUT_OF_MEM "M"

/* Register flags numbers. */

#define FLAG_OVERFLOW_N 1
#define FLAG_NULL_DIV_N 2
#define FLAG_UNK_COMMAND_N 3
#define FLAG_IGNORE_N 4
#define FLAG_OUT_OF_MEM_N 5

#define FLAG_DEFAULT_VALUE 0

/* Signs. */

#define MINUS "-"
#define PLUS "+"

/* Maximal file name length. */
#define FILE_NAME_MAX_LEN 255
#define READ_BIN "rb"
#define WRITE_BIN "wb"
