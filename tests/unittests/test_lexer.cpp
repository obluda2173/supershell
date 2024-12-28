#include "minishell.h"
#include "test_main.hpp"
#include <vector>

struct TestTokenizeParams {
	char* line;
	t_token *want_tokens;
};

class TestTokenizer : public::testing::TestWithParam<TestTokenizeParams>{};

TEST_P(TestTokenizer, firstTests) {
	TestTokenizeParams params = GetParam();
	t_token* tokens = tokenize(params.line);
	EXPECT_EQ(tokens, params.want_tokens);
}

t_token *create_tokens(std::vector<std::string> contents, std::vector<int> types) {
	EXPECT_EQ(contents.size(), types.size());
	t_token* head = NULL;
	for (size_t i = 0; i < contents.size(); i++) {

	}
	return NULL;
}

INSTANTIATE_TEST_SUITE_P(
    LexerTests,
	TestTokenizer,
	testing::Values(
		TestTokenizeParams{(char*)"", nullptr},
		TestTokenizeParams{(char*)"| > <<>>", create_tokens({"|", ">" "<<", ">>"}, {PIPE, REDIRECT_OUT, HERE_DOC, REDIRECT_IN})}
		)
	);
