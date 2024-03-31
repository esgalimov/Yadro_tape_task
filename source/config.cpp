#include "config.hpp"
#include <iostream>

namespace tape_sorter {

namespace config_parser {

config_t parse_config() {
    auto src_folder = std::filesystem::absolute(__FILE__).parent_path().parent_path();
    std::ifstream input{src_folder.append("config.txt").string()};

    config_t config{};
    std::string str;

    while (input >> str) {
        if      (str == "tape_size")       config.tape_sz_   = parse_number(input);
        else if (str == "ram_size")        config.ram_sz_    = parse_number(input);
        else if (str == "read_write_time") config.rw_tm_     = parse_number(input);
        else if (str == "rewind_time")     config.rewind_tm_ = parse_number(input);
        else if (str == "shift_time")      config.shift_tm_  = parse_number(input);

        else throw std::runtime_error("Error: undefind symbols in config file");
    }
    return config;
}

std::size_t parse_number(std::ifstream& input) {
    std::size_t num = 0;

    if (input >> num) return num;

    else throw std::runtime_error("Error: cannot parse number from config file");
}
} // <--- namespace config_parser


std::string read_to_binary(const std::string& filename) {
    std::filesystem::path src_path{filename};
    std::string bin_path = src_path.replace_extension("in.bin").string();

    std::ifstream input{filename};
    std::ofstream output{bin_path, std::ios::binary};

    int num = 0, sz = sizeof(int);

    if (input.is_open() && output.is_open()) {
        while (input >> num)
            output.write(reinterpret_cast<char*>(&num), sz);
    }
    else throw std::runtime_error(
        "Error: cannot read ints to binary file, input or(and) output failed to open");
    
    return bin_path;
}

std::string get_output_bin(const std::string& filename) {
    std::filesystem::path src_path{filename};
    std::string bin_path = src_path.replace_extension("out.bin").string();

    std::ofstream output{bin_path, std::ios::binary};

    return bin_path;
}

void write_output_numan_readable(const std::string& filename) {
    std::ifstream input{get_output_bin(filename), std::ios::binary};
    std::ofstream output{filename};

    int num = 0, sz = sizeof(int);

    if (input.is_open() && output.is_open()) {
        if (input.read(reinterpret_cast<char*>(&num), sz))
            output << num << " ";
    }
    else throw std::runtime_error(
        "Error: cannot write ints to human-readble file, input or(and) output failed to open");
}
} // <-- namespace tape_sorter