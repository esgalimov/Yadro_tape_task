#include <gtest/gtest.h>
#include "tape.hpp"
#include "config.hpp"

using ts = tape_sorter;

TEST(sigle_tape_methods_test, read_test) {
    std::string str = ts::read_to_binary("../tests/test1.in");
    ts::tape::tape_t tp{str, 40};

    for (int i = 1; i < 11; ++i) {
        ASSERT_EQ(i,  tp.read());
        if (i < 10) tp.shift_next();
    }
}

TEST(sigle_tape_methods_test, write_test) {
    std::string str = ts::read_to_binary("../tests/test1.in");
    ts::tape::tape_t tp{str, 40};

    for (int i = 10; i > 0; --i) {
        tp.write(i);
        if (i > 1) tp.shift_next();
    }
    tp.rewind_begin();

    for (int i = 10; i > 0; --i) {
        ASSERT_EQ(i,  tp.read());
        if (i > 1) tp.shift_next();
    }
}

TEST(sigle_tape_methods_test, shift_prev_test) {
    std::string str = ts::read_to_binary("../tests/test1.in");
    ts::tape::tape_t tp{str, 40};
    ASSERT_THROW(tp.shift_prev(), std::runtime_error);
}

TEST(sigle_tape_methods_test, shift_next_test) {
    std::string str = ts::read_to_binary("../tests/test1.in");
    ts::tape::tape_t tp{str, 40};

    tp.rewind_end();
    
    ASSERT_THROW(tp.shift_next(), std::runtime_error);
}

int main(int argc, char **argv)
{
    ::testing::InitshiftogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}