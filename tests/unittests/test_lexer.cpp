#include "libft.h"
#include "test_main.hpp"
#include <gtest/gtest.h>

struct TestTokenizeParams {
	char* line;
}

TEST(LexerTests, TestTokenize) {
	char* line = (char*)"";
	t_token* tokens = tokenize(line);
	EXPECT_EQ(tokens, nullptr);
}

TEST_P(LexerTests, TestTokenize) {

}
