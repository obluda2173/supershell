#include "test_main.hpp"

TEST_P(ParserTestSuite, ParserTest) {
	ParserTestParams params = GetParam();
	std::vector<t_test_script_node> want_nodes = params.want_nodes;

	t_dllist *tokens = create_token_dllist(params.token_vec);
	if (params.token_vec.size() == 0)
		ASSERT_EQ(nullptr, tokens);

	t_list *script = parse(tokens);
	if (params.want_nodes.size() == 0) {
		ASSERT_EQ(nullptr, script);
		ft_dllstclear(&tokens, free_token);
		return;
	}

	if (want_nodes[0].type == ERROR_NODE) {
		compare_error_node(want_nodes[0], (t_script_node*)script->content);
		ASSERT_EQ(NULL, script->next);
		ft_lstclear(&script, free_script_node);
		ft_dllstclear(&tokens, free_token);
		return;
	}


	ASSERT_NE(script, nullptr);
	t_list *head = script;
	for (t_test_script_node want_node : want_nodes) {
		EXPECT_EQ(CMD_NODE, (*(t_script_node*)head->content).node_type);
		compare_cmd_node(want_node, (*(t_script_node*)head->content).node_data.cmd_node);
		head=head->next;
	}
	ASSERT_EQ(head, nullptr);
	ft_dllstclear(&tokens, free_token);
	ft_lstclear(&script, free_script_node);
}


TEST(ParserTestsSuite, TestPipes) {
	std::vector<t_token> token_vec;

	token_vec.push_back(new_token("echo", BUILTIN));
	token_vec.push_back(new_token("|", PIPE));
	token_vec.push_back(new_token("echo", BUILTIN));
	token_vec.push_back(new_token(NULL, END_OF_FILE));

	t_dllist *tokens = create_token_dllist(token_vec);

	t_list *script = parse(tokens);
	ASSERT_NE(nullptr, script);
	t_script_node *sn = (t_script_node*)script->content;
	ASSERT_EQ(sn->node_type, PIPE_NODE);

	ft_lstclear(&script, free_script_node);
	ft_dllstclear(&tokens, free_token);
}
