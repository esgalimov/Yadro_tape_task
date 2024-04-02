#pragma once

#include <stdexcept>

namespace tape_sorter_exceptions {
    struct config_undefind_symbols : public std::runtime_error {
        config_undefind_symbols() : std::runtime_error("Undefind symbols in config file") {}
    };

    struct config_number_parsing : public std::runtime_error {
        config_number_parsing() : std::runtime_error("Cannot parse number from config file") {}
    };

    struct reading_to_binary : public std::runtime_error {
        reading_to_binary() : std::runtime_error("Cannot read ints to binary file, input or(and) output failed to open") {}
    };

    struct reading_from_binary : public std::runtime_error {
        reading_from_binary() : std::runtime_error("Cannot write ints to human-readble file, input or(and) output failed to open") {}
    };

    struct shift_next : public std::runtime_error {
        shift_next() : std::runtime_error("Tape cannot shift next, magnetic head is at the end of tape") {}
    };

    struct shift_prev : public std::runtime_error {
        shift_prev() : std::runtime_error("Tape cannot shift prev, magnetic head is at the beginning of tape") {}
    };
}