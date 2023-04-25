#include <SAT/sat.h>

#include <libcommon/common.h>

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Using: ./sat <input_filename> <output_filename>\n");
        exit(-1);
    }

    FILE *F_source = NULL, *F_out = NULL;
    sat_open_file(argv[1], argv[2], &F_source, &F_out);

    assert(F_source != NULL);
    assert(F_out != NULL);

    unsigned int strnum = sat_get_fstr(F_source);

    __int16_t command_code = 0, operand = 0, mem_cell = 0;
    char command_name[10] = {};
    // __int16_t mem_dump[MEMORY_SIZE] = {};

    for (unsigned int current_string = 0; current_string != strnum;
         ++current_string) {
        sat_read_next_obj(F_source, &mem_cell, command_name, &operand);
        sat_encode_command(command_name, &command_code);
        sat_write_next_obj(F_out, mem_cell, command_code, operand);
    }
}
