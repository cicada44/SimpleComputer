/* Library for working with RAM.
   All functions return SUCCESS or FAIL (defined in common.h).  */

#pragma once

/* Allocate memory for RAM. */
int sc_memoryInit();

/* Sets up memory cell value */
int sc_memorySet(int address, int value);

/* Writes memory cell stored value. */
int sc_memoryGet(int address, int *value);

/* Saves memory into the file. */
int sc_memorySave(char *filename);

/* Loads memory from the file. */
int sc_memoryLoad(char *filename);

/* Sets flag register. */
int sc_regInit();

/* Sets register bit cell to value. */
int sc_regSet(int reg, int value);

/* Writes bit cell value. */
int sc_regGet(int reg, int *value);

/* Encode command and operand to value. */
int sc_commandEncode(int command, int operand, int *value);

/* Decode value to command and operand. */
int sc_commandDecode(int value, int *command, int *operand);

/* Frees RAM. */
int sc_memoryDelete();

/* Prints binary view of n to stdout. */
void bin(unsigned n);

/* Comparator function for bsearch. */
int comp(const void *n1, const void *n2);
