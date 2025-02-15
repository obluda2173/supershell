#include "test_main.hpp"

auto main(int argc, char *argv[]) -> int {
    ::testing::InitGoogleTest(&argc, argv);

    // ::testing::GTEST_FLAG(filter) = "LexerTests*";
    // ::testing::GTEST_FLAG(filter) = "*ParserTest*";
    // ::testing::GTEST_FLAG(filter) = "ParserTestsCmdsWithRedirections*";
    // ::testing::GTEST_FLAG(filter) = "ParserTestsPipesWithParanthesis/ParserTestSuite.ParserTest/2";
    // ::testing::GTEST_FLAG(filter) = "Lexer*:*Parser*";
    // ::testing::GTEST_FLAG(filter) = "ExecutorTest*";
    // ::testing::GTEST_FLAG(filter) = "*TestPatternMatcherSuite*";
    // ::testing::GTEST_FLAG(filter) = "*wildcardTestSuite*";
    // ::testing::GTEST_FLAG(filter) = "*expandVariables*";
    ::testing::GTEST_FLAG(filter) = "sortArg*";
    return RUN_ALL_TESTS();
}
