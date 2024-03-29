#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>


//! @brief Function reads integers from "human-readable" file into binary file
//! @return Created binary file name
std::string read_to_binary(std::string filename) {
    std::filesystem::path src_path{filename};
    std::string bin_path = src_path.replace_extension(".bin").string();

    std::ifstream input{filename};
    std::ofstream output{bin_path, std::ios::binary};

    int num = 0;

    if (input.is_open() && output.is_open()) {
        while (input >> num)
            output.write(reinterpret_cast<char*>(&num), sizeof(num));
    }
    else throw std::runtime_error(
        "Cannot read ints to binary file, input or(and) output failed to open");
    
    return bin_path;
}