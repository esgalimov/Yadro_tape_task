#include <iostream>
#include "config.hpp"
#include "tape.hpp"
#include "sorter.hpp"
#include <array>

int main(int argc, char* argv[]) try {
    std::string str = tape_sorter::read_to_binary("../tests/test1.in");
    std::cout << str << std::endl;
    tape_sorter::tape::tape_t tp{str, 40};

    std::cout << tp.read() << tp.read() << std::endl;
}
catch (std::exception& exc) {
    std::cerr << "Somethig wrong: " << exc.what() << std::endl;
}