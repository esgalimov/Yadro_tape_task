#include <iostream>
#include "config.hpp"
#include "tape.hpp"
#include "sorter.hpp"

namespace ts  = tape_sorter;
namespace tss = tape_sorter::sorter;
namespace cp  = tape_sorter::config_parser;

int main(int argc, char* argv[]) try {
    if (argc < 3 || argc > 5) {
        std::cerr << "Error: wrong arguments number" << std::endl;
        return 1;
    }
    std::string input, output, config_file;

    if (argc >= 4 && argv[1] == ts::parameters::not_bin_mode) {
        input  = ts::read_to_binary(argv[2]);
        output = ts::get_output_bin(argv[3]);
        if (argc == 5) config_file = argv[4];
    }
    else {
        input = argv[1];
        output = argv[2];
        std::ofstream{output};
        if (argc == 4) config_file =  argv[3];
    }

    if (config_file.empty())
        config_file = std::filesystem::absolute(__FILE__).parent_path() / ts::parameters::default_config;

    cp::config_t config = cp::parse_config(config_file);

    tss::sorter_t sorter{input, output, config};
    std::time_t total_time = sorter.sort();

    std::cout << "Total sorting time: " << total_time << std::endl;

    if (argv[1] == ts::parameters::not_bin_mode) {
        ts::write_output_numan_readable(output, argv[3]);
    }
}
catch (std::exception& exc) {
    std::cerr << "Somethig wrong: " << exc.what() << std::endl;
}