#include "test_main.hpp"

TEST_P(ParserTestSuite, ParserTest) {
	ParserTestParams params = GetParam();
	t_test_script_node want = params.want;

	t_dllist *tokens = create_token_dllist(params.token_vec);
	if (params.token_vec.size() == 0)
		ASSERT_EQ(nullptr, tokens);

	t_script_node *sn = parse(tokens);
	if (params.token_vec.size() == 1 && params.token_vec[0].type == END_OF_FILE) {
		ASSERT_EQ(nullptr, sn);
		ft_dllstclear(&tokens, free_token);
		return;
	}

	if (params.test_type == ERROR_TEST)
		return test_error_cases(want, sn,  tokens);

	if (params.test_type == CMD_TEST)
		return test_cmd_cases(want, sn, tokens);

	if (params.test_type == PIPE_TEST)
		return test_pipe_cases(want, sn, tokens);
}
