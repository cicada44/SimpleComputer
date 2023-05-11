#pragma once

#include <fstream>
#include <iostream>

#include <string>

namespace sbt {

/* Opens file. */
void open_file(
        const char* source,
        const char* out,
        std::fstream& f_source,
        std::fstream& f_out);

/* Returns number of string in file f. */
unsigned get_n_str(std::fstream& f);

/* Processes next string in file. */
void process_next_str(std::fstream& out_file, std::string& str);

/* Precesses INPUT command. */
void process_input(std::fstream& str, std::stringstream& ss);

/* Precesses PRINT command. */
void process_print(std::fstream& str, std::stringstream& ss);

/* Precesses END command. */
void process_end(std::fstream& f);

/* Processes LET command. */
void process_let(std::fstream& out_file, std::stringstream& ss);

/* Processes GOTO command. */
void process_goto(std::fstream& out_file, std::stringstream& ss);

}
