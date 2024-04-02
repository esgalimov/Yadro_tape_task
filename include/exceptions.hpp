#pragma once

#include <stdexcept>

namespace tape_sorter_exceptions {
    struct config_undefind_symbols_error : public std::runtime_error {
        config_undefind_symbols_error() : std::runtime_error("Undefind symbols in config file") {}
    };

    struct config_number_parsing_error : public std::runtime_error {
        config_number_parsing_error() : std::runtime_error("Cannot parse number from config file") {}
    };

    struct reading_to_binary_error : public std::runtime_error {
        reading_to_binary_error() : std::runtime_error("Cannot read ints to binary file, input or(and) output failed to open") {}
    };

    struct reading_from_binary_error : public std::runtime_error {
        reading_from_binary_error() : std::runtime_error("Cannot write ints to human-readble file, input or(and) output failed to open") {}
    };

    struct shift_next_error : public std::runtime_error {
        shift_next_error() : std::runtime_error("Tape cannot shift next, magnetic head is at the end of tape") {}
    };

    struct shift_prev_error : public std::runtime_error {
        shift_prev_error() : std::runtime_error("Tape cannot shift prev, magnetic head is at the beginning of tape") {}
    };

    struct config_values_error : public std::runtime_error {
        config_values_error() : std::runtime_error("Bad ram size or times in config") {}
    };

    struct ram_size_error : public std::runtime_error {
        ram_size_error() : std::runtime_error("Ram too small") {}
    };
}