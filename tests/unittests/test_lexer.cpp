#include "test_main.hpp"

struct TestTokenizeParams {
	std::string line;
	std::vector<t_token> want_tokens;
};

class TestTokenizer : public::testing::TestWithParam<TestTokenizeParams>{};

TEST_P(TestTokenizer, firstTests) {
	TestTokenizeParams params = GetParam();
	t_line_container lc = (t_line_container){params.line.c_str(), 0};

	for (size_t i = 0; i < params.want_tokens.size(); i++) {
		auto want_token = params.want_tokens[i];

		t_token got_token = get_next_token(&lc);

		ASSERT_EQ(got_token.content, want_token.content);
		ASSERT_EQ(got_token.type, want_token.type);
	}
}

INSTANTIATE_TEST_SUITE_P(
	LexerTests, TestTokenizer,
	testing::Values(TestTokenizeParams{"", {{NULL, END_OF_FILE}}},
					TestTokenizeParams{">>",
									   {
										   {">>", REDIRECT_APPEND},
										   {NULL, END_OF_FILE}
										   }}
		)
	);
