#pragma once
#include "tape.hpp"
#include <vector>

namespace tape_sorter {
    namespace cp = config_parser;
namespace sorter {
    using namespace tape;
    template <typename T = int>
        requires std::is_arithmetic_v<T>
    class sorter_t final {
        tape::tape_t<T> itape_;
        tape::tape_t<T> otape_;

        tape::tape_t<T> tmp_tp_1_;
        tape::tape_t<T> tmp_tp_2_;

        std::vector<T> ram_;

        cp::config_t config_;

        std::time_t sort_time_ = 0;

        // void load_tape_into_ram(tape::tape_t<T>& tp) {
        //     while (tp.sz_ >= ram)
        // }

    public:
        sorter_t(const std::string& iname, const std::string& oname, cp::config_t config) :
            itape_(iname, config.tape_sz_), otape_(oname, config.tape_sz_), config_(config) {
                ram_.reserve(config_.ram_sz_);
        }

        
    };
} //<-- namespace sorter
} //<-- namespace tape_sorter
