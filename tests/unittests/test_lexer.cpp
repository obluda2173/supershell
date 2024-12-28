#include "minishell.h"
#include "test_main.hpp"
#include <vector>

struct TestTokenizeParams {
	char* line;
	t_token *want_tokens;
};

struct s_line_container {
	char* line;
	int pos;
} t_line_container;

t_tokenizer new_tokenizer(char* line) {
	t_tokenizer t;
	t.line = line;
	t.pos = 0;
	return t;
}

class TestTokenizer : public::testing::TestWithParam<TestTokenizeParams>{};

TEST_P(TestTokenizer, firstTests) {
	TestTokenizeParams params = GetParam();
	param.line
	param.want_tokens
	t_line_container lc = new_line_container(param.line);
	iterate_over_want_tokens {
		t_token got_token = get_next_token(&tokenizer);
		EXPECT_EQUAL(got_token, want_token)
	}
	
	t_token* tokens = tokenize(params.line);


	EXPECT_EQ(tokens, params.want_tokens);
}

INSTANTIATE_TEST_SUITE_P(
    LexerTests,
	TestTokenizer,
	testing::Values(
		TestTokenizeParams{(char*)"", nullptr},
		TestTokenizeParams{(char*)"| > <<>>", create_tokens({"|", ">" "<<", ">>"}, {PIPE, REDIRECT_OUT, HERE_DOC, REDIRECT_IN})}
		)
	);
