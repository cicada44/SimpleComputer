/*
  Library for working with RAM.
  All functions return SUCCESS or FAIL (defined in common.h).
*/

#include <common/common.h>
#include <inttypes.h>
#include <libcomputer/comp.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#define ZERO_BIT 0x0
#define MASK_DECODE_COMMAND 0x7f

// Flag register
static int flags = 0;

// RAM
static int16_t* memory = NULL;

// Commands
static int commands[] = {
        10, 11, 20, 21, 30, 31, 32, 33, 40, 41, 42, 43, 51,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
        65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
};

// Initializes RAM.
int sc_memoryInit()
{
    memory = calloc(MEMORY_SIZE, sizeof(int));

    if (memory == NULL) {
        return FAIL;
    }

    for (int x = MIN_MEMORY_ADDRESS; x != MEMORY_SIZE; ++x) {
        sc_memorySet(x, DEFAULT_MEMORY_VALUE);
    }

    return SUCCESS;
}

// Sets up memory cell value.
int sc_memorySet(int address, int value)
{
    if (address < MIN_MEMORY_ADDRESS || address > MAX_MEMORY_ADDRESS) {
        sc_regSet(M_REGISTER_FLAG, ONE_BIT);
        return FAIL;
    }

    memory[address] = value;

    return SUCCESS;
}

// Writes memory cell stored value.
int sc_memoryGet(int address, int* value)
{
    if (value == NULL || address < MIN_MEMORY_ADDRESS
        || address > MAX_MEMORY_ADDRESS) {
        sc_regSet(M_REGISTER_FLAG, ONE_BIT);
        return FAIL;
    }

    *value = memory[address];

    return SUCCESS;
}

// Saves memory into the file.
int sc_memorySave(char* filename)
{
    FILE* output_file = fopen(filename, WRITE_BIN);

    if (output_file == NULL || memory == NULL) {
        return FAIL;
    }

    if (fwrite(memory, sizeof(int), MEMORY_SIZE, output_file) != MEMORY_SIZE) {
        fclose(output_file);
        return FAIL;
    }

    fclose(output_file);

    return SUCCESS;
}

// Loads memory from the file.
int sc_memoryLoad(char* filename)
{
    FILE* input_file = fopen(filename, READ_BIN);

    if (input_file == NULL || memory == NULL) {
        return FAIL;
    }

    if (fread(memory, sizeof(int), MEMORY_SIZE, input_file) != MEMORY_SIZE) {
        fclose(input_file);
        return FAIL;
    }

    fclose(input_file);

    return SUCCESS;
}

// Sets up flag register.
int sc_regInit()
{
    flags = 0;
    return SUCCESS;
}

// Sets up register bit cell to value.
int sc_regSet(int reg, int value)
{
    if (reg < MIN_REGISTER_NUM || reg > MAX_REGISTER_NUM
        || (value != FALSE_BIT_VALUE && value != TRUE_BIT_VALUE)) {
        return FAIL;
    }

    (value == ONE_BIT) ? (flags |= (MIN_SHIFT << (reg - MIN_SHIFT)))
                       : (flags &= (~(MIN_SHIFT << (reg - MIN_SHIFT))));

    return SUCCESS;
}

// Writes bit cell value.
int sc_regGet(int reg, int* value)
{
    if (reg < MIN_REGISTER_NUM || reg > MAX_REGISTER_NUM || value == NULL) {
        return FAIL;
    }

    *value = (flags >> (reg - MIN_SHIFT)) & ONE_BIT;

    return SUCCESS;
}

// Encodes command and operand to value.
int sc_commandEncode(int command, int operand, int* value)
{
    if (bsearch(&command,
                commands,
                sizeof(commands) / sizeof(commands[0]),
                sizeof(commands[0]),
                comp)
                == NULL
        || operand > MAX_COMMAND_OPERATOR_VALUE
        || command > MAX_COMMAND_OPERATOR_VALUE || value == NULL) {
        return FAIL;
    }

    *value = ZERO_BIT;

    *value = command << SHIFT_DECODE;
    *value |= operand;

    return SUCCESS;
}

// Decodes value to command and operand.
int sc_commandDecode(int value, int* command, int* operand)
{
    if (command == NULL || operand == NULL) {
        sc_regSet(E_REGISTER_FLAG, TRUE_BIT_VALUE);
        return FAIL;
    }

    *operand = *command = ZERO_BIT;

    *operand = value & MASK_DECODE_COMMAND;
    value >>= SHIFT_DECODE;
    *command = value;

    return SUCCESS;
}

// Deletes RAM.
int sc_memoryDelete()
{
    if (memory == NULL) {
        return FAIL;
    }
    free(memory);

    return SUCCESS;
}

// Binary view for unsigned int.
void bin(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

// Comparator function for bsearch.
int comp(const void* n1, const void* n2)
{
    return (*(int*)n1 - *(int*)n2);
}
