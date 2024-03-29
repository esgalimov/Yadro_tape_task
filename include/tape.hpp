#pragma once
#include <fstream>

namespace tape_sorter {
namespace tape {
    template <typename T = int>
        requires std::is_arithmetic_v<T>
    class tape_t final {
        static constexpr int T_sz_ = sizeof(T);
        std::fstream file_;
        std::size_t sz_;

    public:
        tape_t(const std::string& filename, std::size_t sz) : 
            file_(filename, std::ios::binary | std::ios::in | std::ios::out),
            sz_(sz - sz % T_sz_) {}

        void rewind_begin() {
            file_.seekg(0);
        }

        void rewind_end() {
            file_.seekg(sz_ - T_sz_);
        }

        T read() {
            T val = 0;
            file_.read(reinterpret_cast<char*>(&val), T_sz_);
            
            shift_prev();
            return val;
        }

        void write(T val) {
            file_.write(reinterpret_cast<char*>(&val), T_sz_);
            shift_prev();
        }

        void shift_next() {
            if (file_.tellg() < sz_ - T_sz_)
                file_.seekg(T_sz_, std::ios_base::cur);
            else throw std::runtime_error(
                "Error: tape cannot shift next, magnetic head is at the end of tape");
        }

        void shift_prev() {
            if (file_.tellg() != 0)
                file_.seekg(-T_sz_, std::ios_base::cur);
            else throw std::runtime_error(
                "Error: tape cannot shift prev, magnetic head is at the beginning of tape");
        }
    };
   
} //<-- namespace tape
} //<-- namespace tape_sorter