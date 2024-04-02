#pragma once
#include <fstream>
#include <string>

namespace tape_sorter {
namespace config_parser {
struct config_t
{
    std::size_t tape_sz_   = 0; 
    std::size_t ram_sz_    = 0; 
    std::size_t rw_tm_     = 0; 
    std::size_t rewind_tm_ = 0; 
    std::size_t shift_tm_  = 0; 
};

config_t parse_config(const std::string& filename);

std::size_t parse_number(std::ifstream& input);

} // <--- namespace config_parser

namespace parameters {
    constexpr std::string_view not_bin_mode   = "-nb",
                               default_config = "config.txt";
}

std::string read_to_binary(const std::string& filename);

std::string get_output_bin(const std::string& filename);

void write_output_numan_readable(const std::string& iname, const std::string& oname);

} // <-- namespace tape_sorter