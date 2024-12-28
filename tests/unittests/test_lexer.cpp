#include "test_main.hpp"
extern "C" {
#include "lexer.h"
}

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

		t_token *got_token = get_next_token(&lc);

		EXPECT_STREQ(got_token->content, want_token.content);
		EXPECT_EQ(got_token->type, want_token.type);
		free_token(got_token);
	}
}

t_token new_token(const char* content, token_type type) {
	return (t_token){(char*)content, type};

}

INSTANTIATE_TEST_SUITE_P(
	LexerTests, TestTokenizer,
	testing::Values(TestTokenizeParams{"", {{NULL, END_OF_FILE}}},
					TestTokenizeParams{
						">>", {
							new_token(">>", REDIRECT_APPEND),
							new_token(NULL, END_OF_FILE),
						}},
					TestTokenizeParams{
						"<<", {
							new_token("<<", HERE_DOC),
							new_token(NULL, END_OF_FILE),
						}}
		)
	);
