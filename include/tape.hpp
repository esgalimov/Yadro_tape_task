#pragma once
#include <fstream>
#include "exceptions.hpp"

namespace tape_sorter {
namespace tape {
    template <typename T = int>
        requires std::is_arithmetic_v<T>
    class tape_t final {
        std::fstream file_;
        std::size_t  sz_;
        static constexpr int T_sz_ = sizeof(T);

        std::size_t pos = 0;
        
    public:
        tape_t() = default;

        tape_t(const std::string& filename, std::size_t sz) : 
            file_(filename, std::ios::binary | std::ios::in | std::ios::out),
            sz_( sz + T_sz_ - sz % T_sz_) {
                file_.exceptions(std::fstream::failbit | std::fstream::badbit);
        }

        void init_stream(const std::string& filename, std::size_t sz, bool trunc = true) {
            file_.close();

            std::ios_base::openmode opmode = std::ios::binary | std::ios::in | std::ios::out;
            if (trunc) opmode |= std::ios::trunc;

            file_.open(filename, opmode);
            file_.exceptions(std::fstream::failbit | std::fstream::badbit);
            sz_ = sz + T_sz_ - sz % T_sz_;
        }

        void rewind_begin() { file_.seekg(0); }

        void rewind_end()   { file_.seekg(sz_ - T_sz_); }

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
            if (!is_end()) file_.seekg(T_sz_, std::ios_base::cur);
            else throw tape_sorter_exceptions::shift_next();
        }

        void shift_prev() {
            if (!is_begin()) file_.seekg(-T_sz_, std::ios_base::cur);
            else throw tape_sorter_exceptions::shift_prev();
        }

        std::size_t size() { return sz_; }

        void save_pos()  { pos = file_.tellg() / T_sz_; }

        bool check_pos() { return pos == file_.tellg() / T_sz_; }

        std::size_t get_pos() { return file_.tellg() / T_sz_; }
        std::size_t get_saved_pos() { return pos; }
        bool fail() { return file_.fail(); }

        bool is_end()   { return file_.tellg() >= sz_ - T_sz_; }
        bool is_begin() { return file_.tellg() == 0; }
    };
   
} //<-- namespace tape
} //<-- namespace tape_sorter