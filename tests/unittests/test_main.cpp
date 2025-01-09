#include "test_main.hpp"

auto main(int argc, char *argv[]) -> int {
    ::testing::InitGoogleTest(&argc, argv);

    // ::testing::GTEST_FLAG(filter) = "LexerTests*";
    // ::testing::GTEST_FLAG(filter) = "ParserTest*";
    // ::testing::GTEST_FLAG(filter) = "Lexer*:*Parser*";
    // ::testing::GTEST_FLAG(filter) = "ExecutorTests*";
    return RUN_ALL_TESTS();
}
