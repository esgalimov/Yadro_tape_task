#pragma once
#include "tape.hpp"
#include <vector>

namespace tape_sorter {
namespace sorter {
    template <typename T = int>
        requires std::is_arithmetic_v<T>
    class sorter_t final {
        tape::tape_t<T> itape_;
        tape::tape_t<T> otape_;

        std::vector<T> ram_;

        std::array<tape_t, 2> tmp_tapes_;

    public:
        sorter(std::size_t ram_sz, const std::string& iname, 
                                   const std::string& oname) :
            itape_(iname), otape_(oname) {
                ram_.reserve(ram_sz);
        }
    };
} //<-- namespace sorter
} //<-- namespace tape_sorter
