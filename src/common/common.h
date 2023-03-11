// ALL
#define FAIL -1
#define SUCCESS 0
#define READ_BIN "rb"
#define WRITE_BIN "wb"
#define ONE_BIT 0x1
#define NEWLINE "\n"
#define BITS_IN_BYTE 8

// REGISTER
#define MIN_REGISTER_NUM 1
#define TRUE_BIT_VALUE 1
#define FALSE_BIT_VALUE 0
#define MAX_REGISTER_NUM 5
#define MIN_SHIFT 1
#define SHIFT_DECODE 7

// MEMORY
#define MEMORY_SIZE 100
#define MIN_MEMORY_ADDRESS 0
#define MAX_MEMORY_ADDRESS 99
#define DEFAULT_MEMORY_VALUE 0
#define MEMORY_MAX_CELL_VALUE 0x3fff
#define MEMORY_MIN_CELL_VALUE 0x0

// OUTPUT
#define TITLE_MEMORY " Memory "
#define TITLE_ACCUM " accumulator "
#define TITLE_COUNTER " instructionCounter "
#define TITLE_OPERATION " Operation "
#define TITLE_FLAGS " Flags "
#define TITLE_KEYS " Keys: "

// Keys
#define TITLE_KEYS_LOAD "l  - load"
#define TITLE_KEYS_SAVE "s  - save"
#define TITLE_KEYS_RUN "r  - run"
#define TITLE_KEYS_STEP "t  - step"
#define TITLE_KEYS_RESET "i  - reset"
#define TITLE_KEYS_ACCUM "F5 - accumulator"
#define TITLE_KEYS_INSTRCNTER "F6 - instructionCounter"

// Maximal command code.
#define MAX_COMMAND_OPERATOR_VALUE 0x7f

// Default buffer size.
#define BUF_SIZE 15

// Default terminal path and opencode.
#define TERM_PATH "/dev/tty"
#define TERMINAL_OPENCODE 1

// Minimal beginning coordinate.
#define MIN_BEGIN_COORD 0

// Shift difference.
#define SHIFT_DIFF 1

// Number of ints in bigchar.
#define BC_NUM 2

// Registers.

#define FLAG_OVERFLOW "O"
#define FLAG_NULL_DIV "N"
#define FLAG_UNK_COMMAND "E"
#define FLAG_IGNORE "V"
#define FLAG_OUT_OF_MEM "M"

#define FLAG_OVERFLOW_N 1
#define FLAG_NULL_DIV_N 2
#define FLAG_UNK_COMMAND_N 3
#define FLAG_IGNORE_N 4
#define FLAG_OUT_OF_MEM_N 5

// Signs.

#define MINUS "-"

#define PLUS "+"

// Fails.

#define FAIL_OPEN_TERM "[E] FAIL OPENING TERMINAL"
