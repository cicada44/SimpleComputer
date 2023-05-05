#include <SBT/sbt.h>

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void sbt_process_str(
        __attribute_maybe_unused__ FILE* f,
        __attribute_maybe_unused__ FILE* out_file,
        int number_of_out_str,
        char* str)
{
    int num = atoi(strtok(str, " "));
    char* main_command = malloc(sizeof(char) * 100);
    main_command = strtok(NULL, "\n");
    char* first_cmd_word = malloc(sizeof(char) * 100);
    first_cmd_word = strtok(main_command, " ");

    fprintf(out_file, "%02d ", number_of_out_str);
    if (!strcmp(first_cmd_word, "REM")) { /* REM (comment) */
        char* comment_word = malloc(sizeof(char) * 100);
        fprintf(out_file, ";");
        comment_word = strtok(main_command + 4, "\n");
        fprintf(out_file, "%s", comment_word);
    }


    // free(main_command);
    // free(first_cmd_word);

    fprintf(out_file, "\n");
}
