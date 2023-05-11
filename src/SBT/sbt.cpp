#include <SBT/sbt.hpp>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iterator>
#include <libcommon/common.h>
#include <map>
#include <sstream>

namespace sbt {

std::map<std::string, unsigned> variables;

/* Position of the next outputing string in out file. */
unsigned strpos;

/* Position of next variable. */
unsigned varpos = 99;

void open_file(
        const char* source,
        const char* out,
        std::fstream& f_source,
        std::fstream& f_out)
{
    f_source.open(std::string(source));
    f_out.open(std::string(out));
    assert(f_source.is_open() != false);
    assert(f_out.is_open() != false);
}

unsigned get_n_str(std::fstream& file)
{
    return std::count(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>(),
            '\n');
}

void process_input(std::fstream& f, std::stringstream& ss)
{
    std::string name;
    ss >> name;
    variables.insert(std::make_pair(name, varpos));
    f << " READ " << varpos--;
}

void process_print(std::fstream& f, std::stringstream& ss)
{
    std::string name;
    ss >> name;
    f << " WRITE " << variables.at(name);
}

void process_end(std::fstream& f)
{
    f << " HALT 00";
}

void process_let(std::fstream& out_file, std::stringstream& ss)
{
    std::string name;
    ss >> name;

    variables.insert(std::make_pair(name, varpos--));
}

void process_goto(std::fstream& out_file, std::stringstream& ss)
{
    std::string dest_addr;
    ss >> dest_addr;
    out_file << " JUMP " << dest_addr;
}

void process_next_str(std::fstream& out_file, std::string& s)
{
    std::stringstream ss(s);
    std::string num_of_str, action;

    ss >> num_of_str >> action;

    if (action == "REM") { return; }

    if (strpos < 10) out_file << '0';
    out_file << strpos++;

    if (action == "INPUT") {
        process_input(out_file, ss);
    } else if (action == "PRINT") {
        process_print(out_file, ss);
    } else if (action == "END") {
        process_end(out_file);
    } else if (action == "LET") {
        process_let(out_file, ss);
    } else if (action == "GOTO") {
        process_goto(out_file, ss);
    }

    out_file << '\n';
}

} // namespace SBT
