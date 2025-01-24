#include "test_main.hpp"

struct handleLiteralParams {
	const char* word;
	const char* want;
};

class handleLiteralTestSuite : public::testing::TestWithParam<handleLiteralParams>{};

TEST_P(handleLiteralTestSuite, firstTest) {
	handleLiteralParams params = GetParam();
	t_list *ep = get_ep();
	t_data data = {ep, NULL, 0, false};

	char* got = handle_literal((char*)params.word, &data);
	EXPECT_STREQ(params.want, got);
	ft_lstclear(&ep, free_env_var);
	free(got);
}

INSTANTIATE_TEST_SUITE_P(
	handleLiterals,
	handleLiteralTestSuite,
	testing::Values(
		handleLiteralParams{"hello", "hello"},
		handleLiteralParams{"hello2", "hello2"},
		handleLiteralParams{"$PATH", "path_value"},
		handleLiteralParams{"VAR1=\"$PATH path $PATH $path\"", "VAR2=\"path_value path path_value $path\""}
		)
	);
