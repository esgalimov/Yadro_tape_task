#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <ctime>


namespace tape_sorter {
namespace config_parser {
//! @brief Struct to configure tapes and tape sorter
struct config_t
{
    std::size_t tape_sz_   = 0; // tape size in bytes
    std::size_t ram_sz_    = 0; // sorter ram size in bytes
    std::time_t rw_tm_     = 0; // cell read or write time
    std::time_t rewind_tm_ = 0; // rewind time
    std::time_t shift_tm_  = 0; // shift time
};

//! @brief Parse config.txt file from source dir and return config struct
config_t parse_config();

//! @brief Parse number from stream and return it (to use in parse_config function)
std::size_t parse_number(std::ifstream& input);

} // <--- namespace config_parser


//! @brief Function reads integers from "human-readable" file into binary file
//! @return сreated binary file name
std::string read_to_binary(const std::string& filename);

} // <-- namespace tape_sorter