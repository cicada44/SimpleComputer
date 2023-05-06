#pragma once

#include <stdio.h>

/* Set the carriage to the next string. */
void sbt_goto_next_str(FILE* f);

/* Opens file. */
void sbt_open_file(
        const char* source, const char* out, FILE** f_source, FILE** f_out);

/* Returns number of string in file f. */
unsigned int sbt_get_fstr(FILE* f);

/* String main process function. */
void sbt_process_str(FILE* f, FILE* out, int* number_of_out_str, char* str);

/* Returns number of char in str. */
int sbt_get_char(const char* str, char c);

/* Processes INPUT command. */
void sbt_command_input(FILE* out_file, int* number_of_out_str);

/* Processes WRITE command. */
void sbt_command_print(FILE* out_file, int* number_of_out_str);

/* Processes GOTO command. */
void sbt_command_goto(FILE* out_file, int* number_of_out_str);
