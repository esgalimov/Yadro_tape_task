#pragma once
#include "tape.hpp"
#include <vector>

namespace tape_sorter {
    using cp = config_parser;
namespace sorter {
    template <typename T = int>
        requires std::is_arithmetic_v<T>
    class sorter_t final {
        tape::tape_t<T> itape_;
        tape::tape_t<T> otape_;

        std::vector<T> ram_;

        std::array<tape_t, 2> tmp_tapes_;

        cp::config_t config_;

    public:
        sorter(const std::string& iname, const std::string& oname, config_t config) :
            itape_(iname, config.tape_sz_), otape_(oname, config.tape_sz_), config_(config) {
                ram_.reserve(config_.ram_sz_);
        }

        void load_tape();
    };
} //<-- namespace sorter
} //<-- namespace tape_sorter
