#include <libcommon/common.h>
#include <libcomputer/comp.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#define MASK_DECODE_COMMAND 0x7f

/* Flag register. */
static int flags;

/* Virtual RAM. */
static __int16_t* memory = NULL;

int sc_memoryInit()
{
    memory = calloc(MEMORY_SIZE, sizeof(int));

    if (memory == NULL) {
        runtime_error_process(RE.ERROR_MEM_INIT);
        exit(FAIL);
    }

    return SUCCESS;
}

int sc_memorySet(__int8_t address, __int16_t value)
{
    if (address < MEMORY_MIN_ADDRESS || address > MEMORY_MAX_ADDRESS) {
        sc_regSet(FLAG_OUT_OF_MEM_N, BIT_ONE);
        runtime_error_process(RE.ERROR_MEM_SET);
        return FAIL;
    }

    memory[address] = value;

    return SUCCESS;
}

int sc_memoryGet(__int8_t address, __int16_t* value)
{
    if (value == NULL || address < MEMORY_MIN_ADDRESS
        || address > MEMORY_MAX_ADDRESS || memory == NULL) {
        sc_regSet(FLAG_OUT_OF_MEM_N, BIT_ONE);
        return FAIL;
    }

    *value = memory[address];

    return SUCCESS;
}

int sc_memorySave(char* filename)
{
    FILE* output_file = fopen(filename, WRITE_BIN);

    if (output_file == NULL || memory == NULL) {
        (output_file == NULL) ? runtime_error_process(RE.ERROR_OPENING_FILE)
                              : runtime_error_process(RE.ERROR_MEM_INIT);
        return FAIL;
    }

    if (fwrite(memory, sizeof(__int16_t), MEMORY_SIZE, output_file)
        != MEMORY_SIZE) {
        fclose(output_file);
        runtime_error_process(RE.ERROR_FILE_READING);
        return FAIL;
    }

    fclose(output_file);

    return SUCCESS;
}

int sc_memoryLoad(char* filename)
{
    FILE* input_file = fopen(filename, READ_BIN);

    if (input_file == NULL || memory == NULL) {
        (input_file == NULL) ? runtime_error_process(RE.ERROR_OPENING_FILE)
                             : runtime_error_process(RE.ERROR_MEM_INIT);
        return FAIL;
    }

    if (fread(memory, sizeof(__int16_t), MEMORY_SIZE, input_file)
        != MEMORY_SIZE) {
        fclose(input_file);
        runtime_error_process(RE.ERROR_FILE_READING);
        return FAIL;
    }

    fclose(input_file);

    return SUCCESS;
}

int sc_regInit()
{
    flags = 0;
    return SUCCESS;
}

int sc_regSet(__int8_t reg, __int8_t value)
{
    if (reg < MIN_REGISTER_NUM || reg > MAX_REGISTER_NUM
        || (value != BIT_FALSE_VALUE && value != BIT_TRUE_VALUE)) {
        runtime_error_process(RE.ERROR_REG_SET);
        return FAIL;
    }

    (value == BIT_ONE) ? (flags |= (SHIFT_MIN << (reg - SHIFT_MIN)))
                       : (flags &= (~(SHIFT_MIN << (reg - SHIFT_MIN))));

    return SUCCESS;
}

int sc_regGet(__int8_t reg, __int8_t* value)
{
    if (reg < MIN_REGISTER_NUM || reg > MAX_REGISTER_NUM || value == NULL) {
        runtime_error_process(RE.ERROR_MEM_GET);
        return FAIL;
    }

    *value = (flags >> (reg - SHIFT_MIN)) & BIT_ONE;

    return SUCCESS;
}

int sc_commandEncode(__int8_t command, __int8_t operand, __int16_t* value)
{
    *value = BIT_ZERO;
    *value = command << SHIFT_DECODE;
    *value |= operand;

    return SUCCESS;
}

int sc_commandDecode(__int16_t value, __int8_t* command, __int8_t* operand)
{
    if (command == NULL || operand == NULL) {
        sc_regSet(FLAG_UNK_COMMAND_N, BIT_TRUE_VALUE);
        runtime_error_process(RE.ERROR_COMMAND_DECODE);
        return FAIL;
    }

    *operand = *command = BIT_ZERO;

    *operand = value & MASK_DECODE_COMMAND;
    value >>= SHIFT_DECODE;
    *command = value;

    return SUCCESS;
}

int sc_memoryDelete()
{
    if (memory == NULL) {
        runtime_error_process(RE.ERROR_MEM_INIT);
        return FAIL;
    }
    free(memory);

    return SUCCESS;
}

void bin(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

int comp(const void* n1, const void* n2)
{
    return (*(int*)n1 - *(int*)n2);
}
