/*
 *   Simple assembler translator source file
 */

#include <assert.h>

#include <SAT/sat.h>
#include <string.h>

#include <libcomputer/comp.h>

#include <stdio.h>

void sat_goto_next_str(FILE* f)
{
    char c;
    while ((c = fgetc(f)) != '\n') {
    }
}

unsigned int sat_get_fstr(FILE* f)
{
    unsigned int strnum = 0;
    char c;
    while (!feof(f)) {
        c = fgetc(f);
        if (c == '\n') {
            ++strnum;
        }
    }

    fseek(f, 0, SEEK_SET);

    return strnum;
}

void sat_open_file(
        const char* source, const char* out, FILE** f_source, FILE** f_out)
{
    *f_source = fopen(source, "r");
    *f_out = fopen(out, "wb");

    assert(f_source != NULL);
    assert(f_out != NULL);
}

void sat_read_next_obj(
        FILE* source, __int16_t* mem_cell, char* command, __int16_t* operand)
{
    fscanf(source, "%hd", mem_cell);
    fscanf(source, "%s", command);
    fscanf(source, "%hd", operand);

    // printf("%d %s %d\n", *mem_cell, command, *operand);

    sat_goto_next_str(source);
}

void sat_encode_command(const char* name, __int16_t* code)
{
    if (!strcmp(name, "READ")) {
        *code = 0x10;
    } else if (!strcmp(name, "WRITE")) {
        *code = 0x11;
    } else if (!strcmp(name, "LOAD")) {
        *code = 0x20;
    } else if (!strcmp(name, "STORE")) {
        *code = 0x21;
    } else if (!strcmp(name, "ADD")) {
        *code = 0x30;
    } else if (!strcmp(name, "SUB")) {
        *code = 0x31;
    } else if (!strcmp(name, "DIVIDE")) {
        *code = 0x32;
    } else if (!strcmp(name, "MUL")) {
        *code = 0x33;
    } else if (!strcmp(name, "JUMP")) {
        *code = 0x40;
    } else if (!strcmp(name, "JNEG")) {
        *code = 0x41;
    } else if (!strcmp(name, "JZ")) {
        *code = 0x42;
    } else if (!strcmp(name, "HALT")) {
        *code = 0x43;
    } else if (!strcmp(name, "SUB")) {
        *code = 0x10;
    } else if (!strcmp(name, "=")) {
        /* TODO */
    } else {
        fprintf(stderr, "[E] Unknown command, exiting...\n");
        exit(-1);
    }
}

void sat_write_next_obj(
        FILE* file,
        __attribute_maybe_unused__ __int16_t mem_cell,
        __int16_t command_code,
        __int16_t operand)
{
    __int16_t operation;
    sc_commandEncode(command_code, operand, &operation);
    printf("%hd%hd\n", command_code, operand);
    fwrite(&operation, sizeof(operation), 1, file);
}
