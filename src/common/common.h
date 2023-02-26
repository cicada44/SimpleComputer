// ALL
#define FAIL -1
#define SUCCESS 0
#define READ_BIN "rb"
#define WRITE_BIN "wb"
#define ONE_BIT 0x1
#define NEWLINE "\n"
#define BITS_IN_BYTE 8

// REGISTER
#define M_REGISTER_FLAG 3
#define E_REGISTER_FLAG 3
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

// OUTPUT
#define TITLE_MEMORY "Memory"
#define TITLE_ACCUM "accumulator"
#define TITLE_COUNTER "instructionCounter"
#define TITLE_OPERATION "Operation"
#define TITLE_FLAGS "Flags"
#define TITLE_KEYS "Keys: "

// Keys
#define TITLE_KEYS_LOAD "l  -  load"
#define TITLE_KEYS_SAVE "s  -  save"
#define TITLE_KEYS_RUN "r  -  run"
#define TITLE_KEYS_STEP "t  -  step"
#define TITLE_KEYS_RESET "i  -  reset"
#define TITLE_KEYS_ACCUM "F5 -  accumulator"
#define TITLE_KEYS_INSTRCNTER "F6 -  instructionCounter"

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
