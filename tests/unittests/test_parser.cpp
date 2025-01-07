#include "test_main.hpp"

TEST_P(ParserTestSuite, ParserTest) {
	ParserTestParams params = GetParam();
	std::vector<t_test_script_node> want_nodes = params.want_nodes;

	t_dllist *tokens = create_token_dllist(params.token_vec);
	if (params.token_vec.size() == 0)
		ASSERT_EQ(nullptr, tokens);

	t_script_node *script = parse(tokens);
	if (params.want_nodes.size() == 0) {
		ASSERT_EQ(nullptr, script);
		ft_dllstclear(&tokens, free_token);
		return;
	}

	if (params.test_type == ERROR_TEST)
		return test_error_cases(want_nodes, script,  tokens);

	if (params.test_type == CMD_TEST)
		return test_cmd_cases(want_nodes, script, tokens);

	if (params.test_type == PIPE_TEST)
		return test_pipe_cases(want_nodes, script, tokens);
}
