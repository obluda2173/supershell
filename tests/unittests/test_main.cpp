#include "test_main.hpp"

auto main(int argc, char *argv[]) -> int {
    ::testing::InitGoogleTest(&argc, argv);

    // ::testing::GTEST_FLAG(filter) = "ParserTests*";
    ::testing::GTEST_FLAG(filter) = "ExecutorTests*";
    return RUN_ALL_TESTS();
}
