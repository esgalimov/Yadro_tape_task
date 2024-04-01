#include <gtest/gtest.h>
#include "tape.hpp"
#include "config.hpp"

namespace ts = tape_sorter;
namespace cp = tape_sorter::config_parser;

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

// TEST(parse_config_test, test_1) {
//     cp::config_t cnfg = cp::parse_config();

//     ASSERT_EQ(100, cnfg.tape_sz_);
//     ASSERT_EQ(20, cnfg.ram_sz_);
//     ASSERT_EQ(2, cnfg.rw_tm_);
//     ASSERT_EQ(10, cnfg.rewind_tm_);
//     ASSERT_EQ(1, cnfg.shift_tm_);
// }

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}