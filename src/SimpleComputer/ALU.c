#include <SimpleComputer/ALU.h>
#include <SimpleComputer/controlunit.h>
#include <libcommon/common.h>
#include <libcomputer/comp.h>

#include <stdio.h>
#include <unistd.h>

int ALU(__int8_t command, __int8_t operand)
{
    __int16_t memory_cell = 0;
    sc_memoryGet(operand, &memory_cell);

    switch (command) {
    case 0x30:; /* ADD */
        accumulator += memory_cell;
        break;
    case 0x31:; /* SUB */
        accumulator -= memory_cell;
        break;
    case 0x32:; /* DIVIDE */
        accumulator /= memory_cell;
        break;
    case 0x33:; /* MUL */
        accumulator *= memory_cell;
        break;

    default:
        return FAIL;
        break;
    }

    return SUCCESS;
}
