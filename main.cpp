#include <iostream>
#include "config.hpp"

int main(int argc, char* argv[]) try {
    std::cout << read_to_binary("../tests/test1.in") << std::endl;
}
catch (std::exception& exc) {
    std::cerr << "Somethig wrong: " << exc.what() << std::endl;
}