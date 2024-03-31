#include <iostream>
#include "config.hpp"
#include "tape.hpp"
#include "sorter.hpp"
#include <array>

namespace ts  = tape_sorter;
namespace tss = tape_sorter::sorter;
namespace cp  = tape_sorter::config_parser;

int main(int argc, char* argv[]) try {
    cp::config_t config = cp::parse_config();
    std::string input  = ts::read_to_binary("../tests/test3.in"),
                output = ts::get_output_bin("../tests/test3.out");
    std::cout << output << std::endl;
    tss::sorter_t sorter{input, output, config};
    sorter.sort();
    //ts::write_output_numan_readable("../tests/test3.out");
}
catch (std::exception& exc) {
    std::cerr << "Somethig wrong: " << exc.what() << std::endl;
}