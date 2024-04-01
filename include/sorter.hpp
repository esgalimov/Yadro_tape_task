#pragma once
#include "tape.hpp"
#include <vector>
#include <algorithm>

namespace tape_sorter {
    namespace cp = config_parser;
namespace sorter {
    constexpr std::string_view tmp_filename_1 = "tape_task_tmp_1.bin",
                               tmp_filename_2 = "tape_task_tmp_2.bin";

    using namespace tape;
    template <typename T = int>
        requires std::is_arithmetic_v<T>
    class sorter_t final {
        tape::tape_t<T> itape_, otape_, tmp_tp_1_, tmp_tp_2_;

        std::vector<T> ram_;

        const std::time_t rw_tm_, rewind_tm_ , shift_tm_;

        std::time_t sort_time_ = 0;

        using ram_iter = typename std::vector<T>::iterator;

        ram_iter load_tape_into_ram(tape::tape_t<T>& tp, ram_iter start, ram_iter end) {
            while (start != end && start != ram_.end() && !tp.is_end()) {
                *start = tp.read();
                tp.shift_next();
                start++;
            }

            sort_time_ += (start - ram_.begin()) * (rw_tm_ + shift_tm_);

            return start;
        }

        ram_iter load_ram_into_tape(tape::tape_t<T>& tp, ram_iter start, ram_iter end) {
            while (start != end && start != ram_.end() && !tp.is_end()) {
                tp.write(*start);
                tp.shift_next();
                start++;
            }

            sort_time_ += (start - ram_.begin()) * (rw_tm_ + shift_tm_);

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

            //---------------
            std::cout << "tmp1:" << "size = " << tmp_tp_1_.size() << std::endl;
            tmp_tp_1_.rewind_begin();
            while (!tmp_tp_1_.check_pos()) {
                std::cout << tmp_tp_1_.read() << " ";
                //std::cout << tmp_tp_1_.fail() << " ";
                tmp_tp_1_.shift_next();
            }
            //tmp_tp_1_.rewind_begin();

            std::cout << std::endl;

            std::cout << "tmp2:" << "size = " << tmp_tp_2_.size() << std::endl;
            tmp_tp_2_.rewind_begin();
            while (!tmp_tp_2_.check_pos()) {
                std::cout << tmp_tp_2_.read() << " ";
                tmp_tp_2_.shift_next();
            }
            //tmp_tp_2_.rewind_begin();

            std::cout << std::endl;

            //--------------
        }

        void sort_little_tape_in_ram() {
            ram_iter last_loaded = load_tape_into_ram(itape_, ram_.begin(), ram_.end());

            sort_ram(ram_.begin(), last_loaded);

            load_ram_into_tape(otape_, ram_.begin(), last_loaded);
        }

        void merge_tmps() {
            otape_.rewind_begin();
            tmp_tp_1_.rewind_begin();
            tmp_tp_2_.rewind_begin();

            while (!tmp_tp_1_.check_pos() && !tmp_tp_2_.check_pos()) {

                T curr_val1 = tmp_tp_1_.read(), curr_val2 = tmp_tp_2_.read(),
                  prev_val1 = curr_val1,        prev_val2 = curr_val2;

                  std::cout << "guard5" << std::endl;
                
                while (!tmp_tp_1_.check_pos() && !tmp_tp_2_.check_pos() && 
                        prev_val1 <= curr_val1 && prev_val2 <= curr_val2) {
                            std::cout << "guard6" << std::endl;
                            std::cout << "tmp2fail = " << tmp_tp_2_.fail() << std::endl;
                    if (curr_val1 <= curr_val2) {
                        std::cout << "guard7" << std::endl;
                        otape_.write(curr_val1);
                        tmp_tp_1_.shift_next();
                        prev_val1 = curr_val1;
                        curr_val1 = tmp_tp_1_.read();
                        std::cout << "guard8" << std::endl;
                    }
                    else {
                        std::cout << "tmp2fail = " << tmp_tp_2_.fail() << std::endl;
                        std::cout << "guard9" << std::endl;
                        otape_.write(curr_val2);
                        std::cout << "saved pos = " << tmp_tp_2_.get_saved_pos() << std::endl;
                        std::cout << "      pos = " << tmp_tp_2_.get_pos() << std::endl;
                        tmp_tp_2_.shift_next();
                        prev_val2 = curr_val2;
                        curr_val2 = tmp_tp_2_.read();
                        std::cout << "guard10" << std::endl;
                    }
                    std::cout << "tmp2fail = " << tmp_tp_2_.fail() << std::endl;
                    std::cout << "guard11" << std::endl;
                    otape_.shift_next();
                    std::cout << "guard1" << std::endl;
                }
                while (!tmp_tp_1_.check_pos() && prev_val1 <= curr_val1) {
                    otape_.write(curr_val1);
                    tmp_tp_1_.shift_next();
                    prev_val1 = curr_val1;
                    curr_val1 = tmp_tp_2_.read();
                    otape_.shift_next();
                    std::cout << "guard2" << std::endl;
                }
                while (!tmp_tp_2_.check_pos() && prev_val2 <= curr_val2) {
                    otape_.write(curr_val2);
                    tmp_tp_2_.shift_next();
                    prev_val2 = curr_val2;
                    curr_val2 = tmp_tp_2_.read();
                    otape_.shift_next();
                    std::cout << "guard3" << std::endl;
                }
                std::cout << "guard4" << std::endl;
                std::cout << "tmp2fail = " << tmp_tp_2_.fail() << std::endl;
            }

            while (!tmp_tp_1_.check_pos()) {
                T val1 = tmp_tp_1_.read();
                otape_.write(val1);
                tmp_tp_1_.shift_next();
                otape_.shift_next();
                std::cout << "guard5" << std::endl;
            }

            while (!tmp_tp_2_.check_pos()) {
                T val2 = tmp_tp_2_.read();
                otape_.write(val2);
                tmp_tp_2_.shift_next();
                otape_.shift_next();
                std::cout << "guard5" << std::endl;
            }

            //---------------
            std::cout << "otape:" << std::endl;
            otape_.rewind_begin();
            while (!otape_.is_end()) {
                std::cout << otape_.read() << " ";
                otape_.shift_next();
            }
            otape_.rewind_begin();
            std::cout << std::endl;

            //--------------

            sort_time_ += 3 * rewind_tm_ + 2 * otape_.size() * (rw_tm_ + shift_tm_);
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

            sort_time_ += 3 * rewind_tm_ + 2 * otape_.size() * (rw_tm_ + shift_tm_);

             //---------------
            std::cout << "tmp1:" << "size = " << tmp_tp_1_.size() << std::endl;
            tmp_tp_1_.rewind_begin();
            while (!tmp_tp_1_.check_pos()) {
                std::cout << tmp_tp_1_.read() << " ";
                //std::cout << tmp_tp_1_.fail() << " ";
                tmp_tp_1_.shift_next();
            }
            //tmp_tp_1_.rewind_begin();

            std::cout << std::endl;

            std::cout << "tmp2:" << "size = " << tmp_tp_2_.size() << std::endl;
            tmp_tp_2_.rewind_begin();
            while (!tmp_tp_2_.check_pos()) {
                std::cout << tmp_tp_2_.read() << " ";
                tmp_tp_2_.shift_next();
            }
            //tmp_tp_2_.rewind_begin();

            std::cout << std::endl;

            //--------------

        }

    public:
        sorter_t(const std::string& iname, const std::string& oname, cp::config_t config) :
            itape_(iname, config.tape_sz_), otape_(oname, config.tape_sz_),
            rw_tm_(config.rw_tm_), rewind_tm_(config.rewind_tm_), shift_tm_(config.shift_tm_) 
        {
            ram_.resize(config.ram_sz_ / sizeof(T));

            auto tmp_path = std::filesystem::temp_directory_path();

            tmp_tp_1_.init_stream(tmp_path / tmp_filename_1, config.tape_sz_);
            tmp_tp_2_.init_stream(tmp_path / tmp_filename_2, config.tape_sz_);
        }

        std::time_t sort() {
            if (itape_.size() < ram_.size()) 
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
