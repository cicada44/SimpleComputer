#include <stdio.h>

#include <assert.h>
#include <stdlib.h>

#include <SBT/sbt.h>

int main(int argc, char* argv[])
{
    printf("BEGIN OF TRANSLATION\n");

    if (argc != 3) {
        fprintf(stderr, "Using: ./sbt <input_filename> <output_filename>\n");
        exit(-1);
    }

    FILE *F_source = NULL, *F_out = NULL;
    sbt_open_file(argv[1], argv[2], &F_source, &F_out);

    int file_str_num = sbt_get_fstr(F_source);
    int num_of_out_str = 0;
    char actual_str[100];

    for (int i = 0; i != file_str_num; ++i) {
        fgets(actual_str, sizeof(actual_str), F_source);
        sbt_process_str(F_source, F_out, num_of_out_str, actual_str);

        ++num_of_out_str;
    }
}
