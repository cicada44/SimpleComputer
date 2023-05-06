#include <SBT/sbt.h>

#include <assert.h>
#include <ctype.h>
#include <libcommon/common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct variable {
    char name[1];
    int position;
    __int16_t value;
} variable;

variable variables[MEMORY_SIZE];

static int variable_pos = 99;

int sbt_get_variable_pos(const char* name)
{
    for (int i = 0; i != 100; ++i) {
        if (!strcmp(variables[i].name, name)) {
            return variables[i].position;
        }
    }

    return -1;
}

int sbt_get_char(const char* str, char c)
{
    int cnt = 0;

    for (size_t i = 0; i != strlen(str); ++i)
        if (str[i] == c)
            ++cnt;

    return cnt;
}

void sbt_goto_next_str(FILE* f)
{
    char c;
    while ((c = fgetc(f)) != '\n') {
    }
}

void sbt_open_file(
        const char* source, const char* out, FILE** f_source, FILE** f_out)
{
    *f_source = fopen(source, "r");
    *f_out = fopen(out, "w");

    assert(f_source != NULL);
    assert(f_out != NULL);
}

unsigned int sbt_get_fstr(FILE* f)
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

void sbt_command_input(FILE* out_file, int* number_of_out_str)
{
    fprintf(out_file, "%02d READ", (*number_of_out_str)++);

    strcpy(variables[variable_pos].name, strtok(NULL, "\n"));
    variables[variable_pos].position = variable_pos;
    variables[variable_pos].value = 0;

    fprintf(out_file, " %d", variable_pos--);
}

void sbt_command_print(FILE* out_file, int* number_of_out_str)
{
    char operand[1];

    strcpy(operand, strtok(NULL, "\n"));

    printf("%s\n", operand);

    fprintf(out_file,
            "%02d WRITE %02d",
            (*number_of_out_str)++,
            sbt_get_variable_pos(operand));
}

void sbt_process_str(
        __attribute_maybe_unused__ FILE* f,
        __attribute_maybe_unused__ FILE* out_file,
        int* number_of_out_str,
        char* str)
{
    int num = atoi(strtok(str, " "));

    char* main_command = malloc(sizeof(char) * 100);
    assert(main_command != NULL);
    strcpy(main_command, strtok(NULL, "\n"));

    char* first_cmd_word = malloc(sizeof(char) * 100);
    assert(first_cmd_word != NULL);
    strcpy(first_cmd_word, strtok(main_command, " "));

    if (!strcmp(first_cmd_word, "INPUT")) { /* INPUT (READ) */
        sbt_command_input(out_file, number_of_out_str);
    } else if (!strcmp(first_cmd_word, "PRINT")) {
        sbt_command_print(out_file, number_of_out_str);
    } else {
        free(main_command);
        free(first_cmd_word);
        return;
    }

    free(main_command);
    free(first_cmd_word);

    fprintf(out_file, "\n");
}
