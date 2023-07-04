//-----------------------------------------------------------------------------
//
// Header for libcomp.a library. Contains functions for working with RAM.
//
//-----------------------------------------------------------------------------

#pragma once

#include <stdlib.h>

/* Allocate memory for RAM. */
int sc_memoryInit();

/* Sets up memory cell value */
int sc_memorySet(__int8_t address, __int16_t value);

/* Writes memory cell stored value. */
int sc_memoryGet(__int8_t address, __int16_t* value);

/* Saves memory into the file. */
int sc_memorySave(char* filename);

/* Loads memory from the file. */
int sc_memoryLoad(char* filename);

/* Sets flag register. */
int sc_regInit();

/* Sets register bit cell to value. */
int sc_regSet(__int8_t reg, __int8_t value);

/* Writes bit cell value. */
int sc_regGet(__int8_t reg, __int8_t* value);

/* Encode command and operand to value. */
int sc_commandEncode(__int8_t command, __int8_t operand, __int16_t* value);

/* Decode value to command and operand. */
int sc_commandDecode(__int16_t value, __int8_t* command, __int8_t* operand);

/* Frees RAM. */
int sc_memoryDelete();

/* Prints binary view of n to stdout. */
void bin(unsigned n);

/* Comparator function for bsearch. */
int comp(const void* n1, const void* n2);
