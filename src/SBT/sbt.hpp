//-----------------------------------------------------------------------------
//
// Header for SimpleBasic. Contains functions for working with SBT.
//
//-----------------------------------------------------------------------------

#pragma once

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <vector>

#include <string>

namespace sbt {

extern std::map<int, int> adresses;
extern std::list<int> wrong_adresses;

/* Opens file. */
void open_file(const char* source, const char* out, std::fstream& f_source, std::fstream& f_out);

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

/* Return 0, 1 or 2 depends of sign. */
int check_priority(const std::string& s);

/* Returns RPN expression. */
std::string RPN_translate(std::stringstream& ss);

/* Writes variable to output file. */
void add_number(std::fstream& f, const std::string& str);

/* Writes variables to output file in the end. */
void write_left(std::fstream& f);

/* Processes IF command. */
void process_if(std::fstream& out_file, std::stringstream& ss);

/* Process '=' sign. */
void process_if_equal(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f,
        std::string act_after_if);

/* Process '>' sign. */
void process_if_greater(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f,
        std::string act_after_if);

/* Process '<' sign. */
void process_if_less(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f,
        std::string act_after_if);

/* Function to process GOTO problem. */
void translate_final_file(std::ifstream& source, std::ofstream& out);

}
