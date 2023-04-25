/*
 *   Simple assembler translator header
 */

#pragma once

#include <stdio.h>

/* Returns number of strings in file f. */
unsigned int sat_get_fstr(FILE* f);

/* Check is the file available and write them if them're. Otherwise - exit(-1).
 */
void sat_open_file(
        const char* source, const char* out, FILE** f_source, FILE** f_out);

/* Read memory cell number, command, operand. */
void sat_read_next_obj(
        FILE* source, __int16_t* mem_cell, char* command, __int16_t* operand);

/* Encode command to binary. */
void sat_encode_command(const char* name, __int16_t* code);

/* Write command into output file. */
void sat_write_next_obj(
        FILE* file,
        __int16_t mem_cell,
        __int16_t command_code,
        __int16_t operand);
