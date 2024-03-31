#pragma once
#include "tape.hpp"
#include <vector>
#include <algorithm>

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

        const std::time_t rw_tm_, rewind_tm_ , shift_tm_;

        std::time_t sort_time_ = 0;

        using ram_iter = typename std::vector<T>::iterator;

        ram_iter load_tape_into_ram(tape::tape_t<T>& tp, ram_iter start, ram_iter end) {
            for (start != end && !tp.is_end()) {
                *start = itape_.read();
                itape_.shift_next();
                start++;
            }

            sort_time += (start - ram_.begin()) * (rw_tm_ + shift_tm_);

            return ++start;
        }

        ram_iter load_ram_into_tape(tape::tape_t<T>& tp, ram_iter start, ram_iter end) {
            for (start != end && !tp.is_end()) {
                itape_.write(*start);
                itape_.shift_next();
                start++;
            }

            sort_time += (start - ram_.begin()) * (rw_tm_ + shift_tm_);

            return ++start;
        }

        void sort_ram(ram_iter start, ram_iter end) { std::sort(start, end); }

        std::size_t sort_parts_in_ram() {
            std::size_t i = 0;
            while (!itape_.is_end()) {
                ram_iter last_loaded = load_tape_into_ram(itape_, ram_.begin(), ram_.end());

                sort_ram(ram_.begin(), last_loaded);

                if (i++ % 2) load_ram_into_tape(tmp_tp_1_, ram_.begin(), last_loaded);
                else         load_ram_into_tape(tmp_tp_2_, ram_.begin(), last_loaded);
            }
            return tmp_tp_1_.pos();
        }

        void sort_little_tape_in_ram() {
            ram_iter last_loaded = load_tape_into_ram(itape_, ram_.begin(), ram_.end());

            sort_ram(ram_.begin(), last_loaded);

            load_ram_into_tape(otape_, ram_.begin(), last_loaded);
        }

        void merge_tmps(std::size_t tmp_pos_1) {
            tmp_tp_1_.rewind_begin();
            tmp_tp_2_.rewind_beign();

            tmp_pos_2 = otape_.size() - tmp_pos_1 - 1;

            while (tmp_pos_1 > 0 && tmp_pos_2 > 0) {
                T val1 = tmp_tp_1_.read(), val2 = tmp_tp_2_.read();

                if (val1 < val2) {
                    otape_.write(val1);
                    tmp_tp_1_.shift_next();
                    tmp_pos_1--;
                }
                else {
                    otape_.write(val2);
                    tmp_tp_2_.shift_next();
                    tmp_pos_2--;
                }
                otape_.shift_next();
            }

            while (tmp_pos_1 > 0) {
                otape_.write(val1);
                tmp_tp_1_.shift_next();
            }

            while (tmp_pos_2 > 0) {
                otape_.write(val2);
                tmp_tp_2_.shift_next();
            }

            sort_time += 2 * rewind_tm_ + 2 * otape_.size() * (rw_tm_ + shift_tm_);
        }

        void load_merged_into_tmps() {

        }

    public:
        sorter_t(const std::string& iname, const std::string& oname, cp::config_t config) :
            itape_(iname, config.tape_sz_), otape_(oname, config.tape_sz_),
            rw_tm_(config.rw_tm_), rewind_tm_(config.rewind_tm_), shift_tm_(config.shift_tm_) 
        {
                ram_.resize(config.ram_sz_ - config.ram_sz_ % sizeof(T));

                auto tmp_path = std::filesystem::temp_directory_path();

                tmp_tp_1_.init_stream(tmp_path.append("tape_task_tmp_1.bin"), config.tape_sz_);
                tmp_tp_2_.init_stream(tmp_path.append("tape_task_tmp_2.bin"), config.tape_sz_);
        }

        void sort() {
            if (itape_.size() < ram_.size()) 
                sort_little_tape_in_ram();

            sort_parts_in_ram();
        }
        
    };
} //<-- namespace sorter
} //<-- namespace tape_sorter
