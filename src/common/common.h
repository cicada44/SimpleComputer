// ALL
#define FAIL_CODE -1
#define SUCCESS_CODE 0
#define READ_BIN "rb"
#define WRITE_BIN "wb"
#define ONE_BIT 0x1
#define NEWLINE "\n"
#define BITS_IN_BYTE 8

// TERM
#define RESET_COLOR "\033[0m"
#define TERM_PATH "/dev/tty"
#define BEGINNING_POS 1
#define CLEAR_TERM "\E[H\E[J"
#define MIN_POS 0
#define DEFAULT_TERMINAL_CODE 1

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
#define BEGIN_COORD_X 1
#define BEGIN_COORD_Y 1
#define END_COORD_X 10
#define END_COORD_Y 60
#define TITLE_CONTAINS 1
#define TITLE_MEMORY "Memory"
#define TITLE_ACCUM "accumulator"
#define TITLE_COUNTER "instructionCounter"
#define TITLE_OPERATION "Operation"
#define TITLE_FLAGS "Flags"
#define TITLE_KEYS "Keys"
#define DEFAULT_FIELDS_LENGHT 17
#define DEFAULT_FIELDS_SHIFT 3
#define MEMORY_CELL_LEN 45
#define MEMORY_CELL_NUMBER_SHIFT 9

#define MAX_COMMAND_OPERATOR_VALUE 0x7f
