#include "test_main.hpp"

auto main(int argc, char *argv[]) -> int {
	::testing::InitGoogleTest(&argc, argv);


    ft_putstr_fd((char*)"alles gut", STDOUT_FILENO);
    // ::testing::GTEST_FLAG(filter) = "bresPlotlineSimpleTest*";
    // ::testing::GTEST_FLAG(filter) = "wireframeTest*";
    // ::testing::GTEST_FLAG(filter) = "parseMapTest*";
    // ::testing::GTEST_FLAG(filter) = "mapToVectorsTest*";
    // ::testing::GTEST_FLAG(filter) = "scaleTest*";

    return RUN_ALL_TESTS();
}
