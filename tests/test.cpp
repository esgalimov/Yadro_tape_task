#include <gtest/gtest.h>
#include "tape.hpp"
#include "config.hpp"
#include "sorter.hpp"

namespace ts = tape_sorter;
namespace cp = tape_sorter::config_parser;

namespace sorter_tape_tests {
    std::string get_test_filename(const std::string& name) {
        return std::filesystem::absolute(__FILE__).parent_path() / "unit" / name;
    }
}

TEST(sigle_tape_methods_test, read_test) {
    std::string str = ts::read_to_binary(sorter_tape_tests::get_test_filename("test1.in"));
    ts::tape::tape_t tp{str, 10 * sizeof(int)};

    for (int i = 1; i < 11; ++i) {
        ASSERT_EQ(i,  tp.read());
        if (i < 10) tp.shift_next();
    }
}

TEST(sigle_tape_methods_test, write_test) {
    std::string str = ts::read_to_binary(sorter_tape_tests::get_test_filename("test1.in"));
    ts::tape::tape_t tp{str, 10 * sizeof(int)};

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
    std::string str = ts::read_to_binary(sorter_tape_tests::get_test_filename("test1.in"));
    ts::tape::tape_t tp{str, 10 * sizeof(int)};

    ASSERT_EQ(true, tp.is_begin());
    ASSERT_THROW(tp.shift_prev(), tape_sorter_exceptions::shift_prev_error);
}

TEST(sigle_tape_methods_test, size_test) {
    std::string str = ts::read_to_binary(sorter_tape_tests::get_test_filename("test1.in"));
    ts::tape::tape_t tp{str, 10 * sizeof(int)};
    ASSERT_EQ(11 * sizeof(int), tp.size());
}

TEST(sigle_tape_methods_test, shift_next_test) {
    std::string str = ts::read_to_binary(sorter_tape_tests::get_test_filename("test1.in"));
    ts::tape::tape_t tp{str, 10 * sizeof(int)};

    tp.rewind_end();

    ASSERT_EQ(true, tp.is_end());
    ASSERT_THROW(tp.shift_next(), tape_sorter_exceptions::shift_next_error);
}

TEST(sigle_tape_methods_test, save_check_pos_test) {
    std::string str = ts::read_to_binary(sorter_tape_tests::get_test_filename("test1.in"));
    ts::tape::tape_t tp{str, 10 * sizeof(int)};
    
    tp.shift_next();
    tp.save_pos();

    ASSERT_EQ(true, tp.check_pos());
}

TEST(config_test, config_values_error) {
    std::string str = sorter_tape_tests::get_test_filename("bad_config_1.txt");
    ASSERT_THROW(cp::config_t config = cp::parse_config(str),  tape_sorter_exceptions::config_values_error);
}

TEST(config_test, config_ok_test) {
    std::string str = sorter_tape_tests::get_test_filename("good_config_1.txt");
    cp::config_t config = cp::parse_config(str);
    ASSERT_EQ(400, config.ram_sz_);
    ASSERT_EQ(2, config.rw_tm_);
    ASSERT_EQ(10, config.rewind_tm_);
    ASSERT_EQ(1, config.shift_tm_);
}

TEST(sorter_test, ram_size_error) {
    std::string input  = ts::read_to_binary(sorter_tape_tests::get_test_filename("test1.in")),
                output = ts::get_output_bin(sorter_tape_tests::get_test_filename("test1.out"));
    cp::config_t config{};
    
    ASSERT_THROW((ts::sorter::sorter_t{input, output, config}),  tape_sorter_exceptions::ram_size_error);
}

TEST(sorter_test, set_files_config_test) {
    std::string input  = ts::read_to_binary(sorter_tape_tests::get_test_filename("test1.in")),
                output = ts::get_output_bin(sorter_tape_tests::get_test_filename("test1.out")),
                confile = sorter_tape_tests::get_test_filename("good_config_1.txt");
    cp::config_t config = cp::parse_config(confile);
    
    ASSERT_NO_THROW((ts::sorter::sorter_t{input, output, config}));
    ts::sorter::sorter_t sorter{input, output, config};

    input  = ts::read_to_binary(sorter_tape_tests::get_test_filename("test2.in")),
    output = ts::get_output_bin(sorter_tape_tests::get_test_filename("test2.out")),
    confile = sorter_tape_tests::get_test_filename("good_config_2.txt");
    config = cp::parse_config(confile);

    ASSERT_NO_THROW(sorter.set_files(input, output));
    ASSERT_NO_THROW(sorter.set_config(config));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}