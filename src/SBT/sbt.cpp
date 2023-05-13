#include <SBT/sbt.hpp>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <libcommon/common.h>
#include <list>
#include <map>
#include <sstream>
#include <stack>

namespace sbt {

std::map<int, int> adresses;

std::list<int> wrong_adresses;

std::string prev_command;

std::map<std::string, int> temp_vars;
std::map<std::string, unsigned> variables;

/* Position of the next outputing string in out file. */
unsigned strpos;

/* Position of next variable. */
unsigned varpos = 99;

void open_file(const char* source, const char* out, std::fstream& f_source, std::fstream& f_out)
{
    f_source.open(std::string(source));
    f_out.open(std::string(out));
    assert(f_source.is_open() != false);
    assert(f_out.is_open() != false);
}

unsigned get_n_str(std::fstream& file)
{
    return std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
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

int check_priority(const std::string& s)
{
    int priority = 0;
    if (s == "/" || s == "*") {
        priority = 2;
    } else if (s == "+" || s == "-") {
        priority = 1;
    }

    return priority;
}

std::string RPN_translate(std::stringstream& ss)
{
    std::stack<std::string> helper_stack;
    std::string tmp, final_expression;
    while (ss >> tmp) {
        if (tmp == "(") {
            helper_stack.push(tmp);
        } else if (tmp == ")") {
            std::string pushable = " ";
            do {
                final_expression.append(pushable);
                pushable = helper_stack.top();
                helper_stack.pop();
            } while (pushable != "(");
        } else if (tmp == "*" || tmp == "/" || tmp == "+" || tmp == "-") {
            if ((!helper_stack.empty())
                && (check_priority(tmp) <= check_priority(helper_stack.top()))) {
                final_expression.append(helper_stack.top());
                final_expression.append(" ");
                helper_stack.pop();
            }
            helper_stack.push(tmp);
        } else {
            final_expression.append(tmp);
        }

        final_expression.append(" ");
    }

    while (!helper_stack.empty()) {
        final_expression.append(helper_stack.top());
        helper_stack.pop();
    }

    return final_expression;
}

void add_number(std::fstream& f, const std::string& str)
{
    temp_vars.insert(std::make_pair(str, std::stoi(str)));
    f << " " << strpos++ << " = " << temp_vars.at(str) << '\n';
}

void process_let(std::fstream& out_file, std::stringstream& ss)
{
    std::string expression, final_expression, name, assing_symbol;
    ss >> name >> assing_symbol;

    variables.insert(std::make_pair(name, varpos--));
    unsigned addr_dest = variables.at(name);
    final_expression = RPN_translate(ss);

    std::stringstream calc_ss(final_expression);
    std::stack<std::string> calc_stack;
    std::string tmp;
    while (calc_ss >> tmp) {
        // std::cout << "\n\nVARIABLES\n";
        // for (const auto& c : variables) {
        //     std::cout << c.first << ' ' << c.second << '\n';
        // }
        // std::cout << '\n';
        if (tmp == "-" || tmp == "+" || tmp == "*" || tmp == "/") {
            std::string o1 = calc_stack.top();
            calc_stack.pop();
            std::string o2 = calc_stack.top();
            calc_stack.pop();

            unsigned addr1 = variables.at(o1);
            unsigned addr2 = variables.at(o2);

            if (strpos < 10) out_file << '0';
            out_file << strpos++ << " LOAD " << addr2 << '\n';

            if (tmp == "-") {
                if (strpos < 10) out_file << '0';
                out_file << strpos++ << " SUB " << addr1 << '\n';
            } else if (tmp == "+") {
                if (strpos < 10) out_file << '0';
                out_file << strpos++ << " ADD " << addr1 << '\n';
            } else if (tmp == "*") {
                if (strpos < 10) out_file << '0';
                out_file << strpos++ << " MUL " << addr1 << '\n';
            } else if (tmp == "/") {
                if (strpos < 10) out_file << '0';
                out_file << strpos++ << " DIVIDE " << addr1 << '\n';
            }

            if (strpos < 10) out_file << '0';
            out_file << strpos++ << " STORE " << addr_dest << '\n';
            calc_stack.push(name);
        } else {
            if (std::isdigit(tmp.at(0)) && tmp.at(0) != '0') {
                variables.insert(std::make_pair(tmp, varpos--));
            }
            calc_stack.push(tmp);
        }
    }

    if (calc_stack.size() > 0) {
        if (strpos < 10) out_file << '0';
        out_file << strpos++ << " LOAD " << variables.at(calc_stack.top()) << '\n';
        if (strpos < 10) out_file << '0';
        out_file << strpos++ << " STORE " << variables.at(name) << '\n';
    }
}

void process_goto(std::fstream& out_file, std::stringstream& ss)
{
    std::string dest_addr;
    ss >> dest_addr;
    wrong_adresses.push_back(std::atoi(dest_addr.c_str()));
    dest_addr = "-1";
    out_file << " JUMP " << dest_addr;
}

void process_if_equal(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f,
        std::string act_after_if)
{
    if (std::isdigit(o2.at(0))) { variables.insert(std::make_pair(o2, varpos--)); }
    if (strpos < 10) f << '0';
    f << strpos++ << " LOAD " << variables.at(o2) << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " SUB " << variables.at(o1) << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " JZ " << strpos + 1 << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " JUMP " << strpos + 1 << '\n';
    process_next_str(f, act_after_if);
}

void process_if_greater(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f,
        std::string act_after_if)
{
    if (std::isdigit(o2.at(0))) { variables.insert(std::make_pair(o2, varpos--)); }
    if (strpos < 10) f << '0';
    f << strpos++ << " LOAD " << variables.at(o2) << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " SUB " << variables.at(o1) << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " JNEG " << strpos + 1 << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " JUMP " << strpos + 1 << '\n';
    process_next_str(f, act_after_if);
}

void process_if_less(
        const std::string& o1,
        const std::string& o2,
        const std::string& operation,
        std::fstream& f,
        std::string act_after_if)
{
    if (std::isdigit(o2.at(0))) { variables.insert(std::make_pair(o2, varpos--)); }
    if (strpos < 10) f << '0';
    f << strpos++ << " LOAD " << variables.at(o1) << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " SUB " << variables.at(o2) << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " JNEG " << strpos + 1 << '\n';
    if (strpos < 10) f << '0';
    f << strpos++ << " JUMP " << strpos + 1 << '\n';
    process_next_str(f, act_after_if);
}

void process_if(std::fstream& out_file, std::stringstream& ss)
{
    std::string o1, o2, operation;

    ss >> o1 >> operation >> o2;

    std::string expr, tmp;
    while (ss >> tmp) {
        expr.append(tmp);
        expr.append(" ");
    }
    expr.pop_back();

    if (operation == "=") {
        process_if_equal(o1, o2, operation, out_file, expr);
    } else if (operation == "<") {
        process_if_less(o1, o2, operation, out_file, expr);
    } else if (operation == ">") {
        process_if_greater(o1, o2, operation, out_file, expr);
    }
}

void process_next_str(std::fstream& out_file, std::string& s)
{
    if (s.empty()) { return; }

    std::stringstream ss(s);
    std::string num_of_str, action;

    if (prev_command != "IF") {
        ss >> num_of_str >> action;
        adresses.insert(std::make_pair(std::atoi(num_of_str.c_str()), strpos));
    } else {
        ss >> action;
    }

    // std::cout << "NUM OF STR: " << num_of_str << '\t'
    //           << "ACTION: " << action << '\n';

    if (action == "REM") { return; }

    if (strpos < 10 && action != "LET" && action != "IF") out_file << '0';
    if (action != "LET" && action != "IF") out_file << strpos++;

    prev_command = action;

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
        // wrong_adresses.push_back();
    } else if (action == "IF") {
        process_if(out_file, ss);
    }

    if (action != "LET" && action != "IF") out_file << '\n';
}

void write_left(std::fstream& f)
{
    for (const auto& c : variables) {
        if (std::isdigit(c.first.at(0))) {
            f << c.second << " = " << c.first << '\n';
        } else {
            f << c.second << " = " << 0 << '\n';
        }
    }
}

void translate_final_file(std::ifstream& source, std::ofstream& out)
{
    std::stringstream ss;
    std::string num_of_str, action, address, actual;
    while (std::getline(source, actual, '\n')) {
        ss.clear();
        ss.str(actual);
        ss >> num_of_str >> action >> address;

        if (action == "JUMP" && address == "-1") {
            address = std::to_string(adresses.at(*wrong_adresses.begin()));
            wrong_adresses.pop_front();
        }

        out << num_of_str << " " << action << " " << address << '\n';

        // std::cout << actual << '\n';
    }
}

} // namespace SBT
