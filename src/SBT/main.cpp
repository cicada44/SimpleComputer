#include <iostream>

#include <cassert>
#include <fstream>
#include <iterator>

#include <SBT/sbt.hpp>

int main(int argc, char* argv[])
{
    std::cout << "[I] BEGIN OF TRANSLATION [I]\n";

    if (argc != 3) {
        std::cerr << "Using: ./sbt <input_filename> <output_filename>\n";
        exit(-1);
    }

    std::fstream F_source, F_out;
    sbt::open_file(argv[1], argv[2], F_source, F_out);
    std::string inputable;
    while (std::getline(F_source, inputable, '\n')) {
        sbt::process_next_str(F_out, inputable);
    }

    F_source.close();
    F_out.close();

    std::cout << "[I] END OF TRANSLATION [I]\n";
}
