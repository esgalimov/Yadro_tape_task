#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <ctime>


namespace tape_sorter {

namespace config_parser {
struct config_t
{
    std::size_t tape_sz_   = 0;
    std::size_t ram_sz_    = 0;
    std::time_t rw_tm_     = 0;
    std::time_t rewind_tm_ = 0;
    std::time_t shift_tm_  = 0;
};

config_t parse_config() {
    config_t config{};
    std::ifstream input{"config.txt"};

    std::string str;

    while (input >> str) {
        if (str == "tape_size") {
            
        }
    }
}

std::size_t parse_number(std::ifstream& input) {
    if (input >> str) {
        if (str == "=") {
            
        }
        else 
            throw std::runtime_error(
                "Error: undefined symbol after parametr name in config file");
        }
}
} // <--- namespace config_parser

//! @brief Function reads integers from "human-readable" file into binary file
//! @return Created binary file name
std::string read_to_binary(const std::string& filename) {
    std::filesystem::path src_path{filename};
    std::string bin_path = src_path.replace_extension(".bin").string();

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
} // <-- namespace tape_sorter