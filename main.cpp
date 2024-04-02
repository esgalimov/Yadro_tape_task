#include <iostream>
#include "config.hpp"
#include "tape.hpp"
#include "sorter.hpp"

namespace ts  = tape_sorter;
namespace tss = tape_sorter::sorter;
namespace cp  = tape_sorter::config_parser;

int main(int argc, char* argv[]) try {
    if (argc < 3) {
        std::cerr << "Error: too few arguments" << std::endl;
        return 1;
    }
    std::string input, output;

    if (argc >= 4 && argv[1] == ts::sort_flags::bin_mode) {
        input = argv[2];
        output = argv[3];
        std::ofstream{output};
    }
    else {
        input  = ts::read_to_binary(argv[1]);
        output = ts::get_output_bin(argv[2]);
    }

    auto config_path = std::filesystem::absolute(__FILE__).parent_path();
    config_path.append("config.txt");

    cp::config_t config = cp::parse_config(config_path);

    tss::sorter_t sorter{input, output, config};
    std::time_t total_time = sorter.sort();

    std::cout << "Total sorting time: " << total_time << std::endl;

    if (argv[1] != ts::sort_flags::bin_mode) {
        ts::write_output_numan_readable(output, argv[2]);
    }
}
catch (std::exception& exc) {
    std::cerr << "Somethig wrong: " << exc.what() << std::endl;
}