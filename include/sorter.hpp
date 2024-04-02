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
        tape::tape_t<T> itape_, otape_, tmp_tp_1_, tmp_tp_2_;

        std::string tmp_filename_1, tmp_filename_2;

        std::vector<T> ram_;

        std::time_t rw_tm_, rewind_tm_, shift_tm_;

        std::time_t sort_time_ = 0;

        using ram_iter = typename std::vector<T>::iterator;

        void add_time(std::time_t tm) {
            sort_time_ += tm;
        }

        ram_iter load_tape_into_ram(tape::tape_t<T>& tp, ram_iter start, ram_iter end) {
            while (start != end && start != ram_.end() && !tp.is_end()) {
                *start = tp.read();
                tp.shift_next();
                start++;
            }

            add_time((start - ram_.begin()) * (rw_tm_ + shift_tm_));

            return start;
        }

        ram_iter load_ram_into_tape(tape::tape_t<T>& tp, ram_iter start, ram_iter end) {
            while (start != end && start != ram_.end() && !tp.is_end()) {
                tp.write(*start);
                tp.shift_next();
                start++;
            }

            add_time((start - ram_.begin()) * (rw_tm_ + shift_tm_));

            return start;
        }

        void sort_ram(ram_iter start, ram_iter end) { std::sort(start, end); }

        void sort_parts_in_ram() {
            std::size_t i = 0;
            while (!itape_.is_end()) {
                ram_iter last_loaded = load_tape_into_ram(itape_, ram_.begin(), ram_.end());

                sort_ram(ram_.begin(), last_loaded);

                if (i++ % 2 == 0) load_ram_into_tape(tmp_tp_1_, ram_.begin(), last_loaded);
                else              load_ram_into_tape(tmp_tp_2_, ram_.begin(), last_loaded);
            }
            
            tmp_tp_1_.save_pos();
            tmp_tp_2_.save_pos();
        }

        void sort_little_tape_in_ram() {
            ram_iter last_loaded = load_tape_into_ram(itape_, ram_.begin(), ram_.end());

            sort_ram(ram_.begin(), last_loaded);

            load_ram_into_tape(otape_, ram_.begin(), last_loaded);
        }

        void merge_while_sorted_subarrays(T& prev_val1, T& curr_val1, 
                                          T& prev_val2, T& curr_val2) {
            while (!tmp_tp_1_.check_pos() && prev_val1 <= curr_val1) {
                    otape_.write(curr_val1);
                    otape_.shift_next();
                    tmp_tp_1_.shift_next();
                    if (tmp_tp_1_.check_pos()) break;
                    prev_val1 = curr_val1;
                    curr_val1 = tmp_tp_1_.read();
            }

            while (!tmp_tp_2_.check_pos() && prev_val2 <= curr_val2) {
                otape_.write(curr_val2);
                otape_.shift_next();
                tmp_tp_2_.shift_next();
                if (tmp_tp_2_.check_pos()) break;
                prev_val2 = curr_val2;
                curr_val2 = tmp_tp_2_.read();
            }
        }

        void merge_tapes_end() {
            while (!tmp_tp_1_.check_pos()) {
                T val1 = tmp_tp_1_.read();
                otape_.write(val1);
                tmp_tp_1_.shift_next();
                otape_.shift_next();
            }
            while (!tmp_tp_2_.check_pos()) {
                T val2 = tmp_tp_2_.read();
                otape_.write(val2);
                tmp_tp_2_.shift_next();
                otape_.shift_next();
            }
        }

        void single_merge(T& prev_val1, T& curr_val1, 
                          T& prev_val2, T& curr_val2) {
            if (curr_val1 < curr_val2) {
                otape_.write(curr_val1);
                tmp_tp_1_.shift_next();
                if (!tmp_tp_1_.check_pos()) {
                    prev_val1 = curr_val1;
                    curr_val1 = tmp_tp_1_.read();
                }
            }
            else {
                otape_.write(curr_val2);
                tmp_tp_2_.shift_next();
                if (!tmp_tp_2_.check_pos()) {
                    prev_val2 = curr_val2;
                    curr_val2 = tmp_tp_2_.read();
                }
            }
            otape_.shift_next();
        }

        void merge_tmps() {
            otape_.rewind_begin();
            tmp_tp_1_.rewind_begin();
            tmp_tp_2_.rewind_begin();

            T curr_val1 = tmp_tp_1_.read(), curr_val2 = tmp_tp_2_.read();

            while (!tmp_tp_1_.check_pos() && !tmp_tp_2_.check_pos()) {

                T prev_val1 = curr_val1, prev_val2 = curr_val2;

                while (!tmp_tp_1_.check_pos() && !tmp_tp_2_.check_pos() && 
                        prev_val1 <= curr_val1 && prev_val2 <= curr_val2) {
                            
                    single_merge(prev_val1, curr_val1, prev_val2, curr_val2);
                }
                merge_while_sorted_subarrays(prev_val1, curr_val1, prev_val2, curr_val2);
            }

            merge_tapes_end();

            add_time(3 * rewind_tm_ + 2 * otape_.size() * (rw_tm_ + shift_tm_));
        }

        void load_merged_into_tmps() {
            tmp_tp_1_.rewind_begin();
            tmp_tp_2_.rewind_begin();
            otape_.rewind_begin();

            T curr_val = otape_.read(), prev_val = 0;

            while (!otape_.is_end()) {
                do {
                    tmp_tp_1_.write(curr_val);
                    tmp_tp_1_.shift_next();
                    otape_.shift_next();

                    prev_val = curr_val;
                    if (otape_.is_end()) break;
                    curr_val = otape_.read();
                } while (curr_val >= prev_val);

                if (otape_.is_end()) break;

                do {
                    tmp_tp_2_.write(curr_val);
                    tmp_tp_2_.shift_next();
                    otape_.shift_next();
                    prev_val = curr_val;
                    if (otape_.is_end()) break;
                    curr_val = otape_.read();
                } while (curr_val >= prev_val && !otape_.is_end());
            }

            tmp_tp_1_.save_pos();
            tmp_tp_2_.save_pos();

            add_time(3 * rewind_tm_ + 2 * otape_.size() * (rw_tm_ + shift_tm_));
        }

        void generate_tmp_filenames() {
            tmp_filename_1 = unique_tmp_filename_generator(1);
            tmp_filename_2 = unique_tmp_filename_generator(2);
        }

    public:
        sorter_t(const std::string& iname, const std::string& oname, cp::config_t config) :
            rw_tm_(config.rw_tm_), rewind_tm_(config.rewind_tm_), shift_tm_(config.shift_tm_) 
        {
            ram_.resize(config.ram_sz_ / sizeof(T));

            if (ram_.size() == 0) throw tape_sorter_exceptions::ram_size();

            std::size_t tape_sz = std::filesystem::file_size(iname);

            itape_.init_stream(iname, tape_sz, false);
            otape_.init_stream(oname, tape_sz, false);

            auto tmp_path = std::filesystem::temp_directory_path();
            generate_tmp_filenames();

            tmp_tp_1_.init_stream(tmp_path / tmp_filename_1, tape_sz);
            tmp_tp_2_.init_stream(tmp_path / tmp_filename_2, tape_sz);
        }

        ~sorter_t() {
            auto tmp_path = std::filesystem::temp_directory_path();
            std::filesystem::remove(tmp_path / tmp_filename_1);
            std::filesystem::remove(tmp_path / tmp_filename_2);
        }

        sorter_t(const sorter_t& sorter)            = delete;
        sorter_t& operator=(const sorter_t& sorter) = delete;

        sorter_t(sorter_t&& sorter)            = default;
        sorter_t& operator=(sorter_t&& sorter) = default;

        void set_files(const std::string& iname, const std::string& oname) {
            std::size_t tape_sz = std::filesystem::file_size(iname);

            itape_.init_stream(iname, tape_sz, false);
            otape_.init_stream(oname, tape_sz, false);
        }

        void set_config(cp::config_t config) {
            ram_.resize(config.ram_sz_ / sizeof(T));

            if (ram_.size() == 0) throw tape_sorter_exceptions::ram_size();
            
            rw_tm_     = config.rw_tm_;
            rewind_tm_ = config.rewind_tm_; 
            shift_tm_  = config.shift_tm_;
        }

        std::time_t sort() {
            sort_time_ = 0;

            if (itape_.size() <= 4) return sort_time_;

            else if (itape_.size() < ram_.size()) 
                sort_little_tape_in_ram();

            else {
                sort_parts_in_ram();

                while (!tmp_tp_1_.is_begin() && !tmp_tp_2_.is_begin()) {
                    merge_tmps();
                    load_merged_into_tmps();

                    std::cout << "tmp1 pos = " << tmp_tp_1_.get_pos() << std::endl;
                    std::cout << "tmp2 pos = " << tmp_tp_2_.get_pos() << std::endl;
                    std::cout << std::endl;
                }
                
                merge_tmps();
            }
            return sort_time_;
        }
        
    };
} //<-- namespace sorter
} //<-- namespace tape_sorter
