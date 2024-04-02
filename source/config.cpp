#include "config.hpp"
#include <iostream>
#include <limits>
#include <random>
#include "exceptions.hpp"

namespace tape_sorter {

namespace config_parser {

config_t parse_config(const std::string& filename) {
    std::ifstream input{filename};

    config_t config{};
    std::string str;

    while (input >> str) {
        if      (str == "ram_size")        config.ram_sz_    = parse_number(input);
        else if (str == "read_write_time") config.rw_tm_     = parse_number(input);
        else if (str == "rewind_time")     config.rewind_tm_ = parse_number(input);
        else if (str == "shift_time")      config.shift_tm_  = parse_number(input);

        else throw tape_sorter_exceptions::config_undefind_symbols();
    }
    return config;
}

std::size_t parse_number(std::ifstream& input) {
    std::size_t num = 0;

    if (input >> num) return num;

    else throw tape_sorter_exceptions::config_number_parsing();
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
    else throw tape_sorter_exceptions::reading_to_binary();
    
    return bin_path;
}

std::string get_output_bin(const std::string& filename) {
    std::filesystem::path src_path{filename};
    std::string bin_path = src_path.replace_extension("out.bin").string();

    std::ofstream output{bin_path, std::ios::binary};

    return bin_path;
}

void write_output_numan_readable(const std::string& iname, const std::string& oname) {
    std::ifstream input{iname, std::ios::binary};
    std::ofstream output{oname};

    int num = 0, sz = sizeof(int);

    if (input.is_open() && output.is_open()) {
        while (input.read(reinterpret_cast<char*>(&num), sz))
            output << num << " ";
    }
    else throw std::runtime_error(
        "Error: cannot write ints to human-readble file, input or(and) output failed to open");
}

std::string unique_tmp_filename_generator(int index) {
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distrib(std::numeric_limits<int>::min(), 
                                            std::numeric_limits<int>::max());
    
    return std::to_string(index) + std::string{"tape_task"} + std::to_string(distrib(gen));
}
} // <-- namespace tape_sorter