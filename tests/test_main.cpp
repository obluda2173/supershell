#include <gtest/gtest.h>
extern "C" {
	#include "executor.h"
}

auto main(int argc, char *argv[]) -> int {
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

TEST(EchoTests, echoStrings) {
	testing::internal::CaptureStdout();
	execute();
	std::string got = testing::internal::GetCapturedStdout();

	EXPECT_EQ("Hello world!", got);
}
