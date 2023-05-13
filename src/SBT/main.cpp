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
    sbt::write_left(F_out);

    F_source.close();
    F_out.close();

    std::cout << "[I] END OF TRANSLATION [I]\n";

    // for (const auto& c : sbt::adresses) {
    //     std::cout << c.first << '\t' << c.second << '\n';
    // }
    // std::cout << "\n\n\n";
    // for (const auto& c : sbt::wrong_adresses) {
    //     std::cout << c << ' ';
    // }
    // std::cout << '\n';

    /* Renaming file. */
    std::string name_out_file(argv[2]);
    std::rename(argv[2], name_out_file.append(".tmp").c_str());

    /* Translate file. */
    std::ofstream final_out_file(argv[2]);
    std::ifstream source_file(name_out_file);
    sbt::translate_final_file(source_file, final_out_file);

    final_out_file.close();
    source_file.close();

    // std::remove(name_out_file.c_str());
}
