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

int check_priority(const std::string& s);

/* Returns RPN expression. */
std::string RPN_translate(std::stringstream& ss);

void add_number(std::fstream& f, const std::string& str);

void write_left(std::fstream& f);

/* Processes IF command. */
void process_if(std::fstream& out_file, std::stringstream& ss);

void process_if_equal(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f,
        std::string act_after_if);
void process_if_greater(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f);
void process_if_less(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f);

}
