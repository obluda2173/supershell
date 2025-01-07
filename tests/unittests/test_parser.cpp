#include "test_main.hpp"

void test_error_cases(std::vector<t_test_script_node> want_nodes, t_list *script, t_dllist* tokens) {
		compare_error_node(want_nodes[0], (t_script_node*)script->content);
		ASSERT_EQ(NULL, script->next);
		ft_lstclear(&script, free_script_node);
		ft_dllstclear(&tokens, free_token);
}

void test_cmd_cases(std::vector<t_test_script_node> want_nodes, t_list *script, t_dllist* tokens) {
	ASSERT_NE(script, nullptr);
	t_list *head = script;
	for (t_test_script_node want_node : want_nodes) {
		EXPECT_EQ(CMD_NODE, (*(t_script_node*)head->content).node_type);
		EXPECT_EQ(0, (*(t_script_node*)head->content).num_children);
		EXPECT_EQ(nullptr, (*(t_script_node*)head->content).child1);
		compare_cmd_node(want_node, (*(t_script_node*)head->content).node_data.cmd_node);
		head=head->next;
	}
	ASSERT_EQ(head, nullptr);
	ft_dllstclear(&tokens, free_token);
	ft_lstclear(&script, free_script_node);
}

void test_pipe_cases(std::vector<t_test_script_node> want_nodes, t_list *script, t_dllist* tokens) {
	t_test_script_node want = want_nodes[0];
	ASSERT_NE(nullptr, script);
	t_script_node *sn = (t_script_node*)script->content;
	ASSERT_EQ(sn->node_type, PIPE_NODE);
	ASSERT_EQ(sn->num_children, 2);
	ASSERT_NE(nullptr,sn->child1);
	ASSERT_EQ(CMD_NODE, sn->child1->node_type);
	ASSERT_NE(nullptr,sn->child2);
	ASSERT_EQ(CMD_NODE, sn->child2->node_type);

	compare_cmd_node(want.pipe_node_childs[0], (t_cmd_node)sn->child1->node_data.cmd_node);
	compare_cmd_node(want.pipe_node_childs[1], (t_cmd_node)sn->child2->node_data.cmd_node);

	ft_lstclear(&script, free_script_node);
	ft_dllstclear(&tokens, free_token);
}

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

	if (params.test_type == ERROR_TEST)
		return test_error_cases(want_nodes, script,  tokens);

	if (params.test_type == CMD_TEST)
		return test_cmd_cases(want_nodes, script, tokens);

	if (params.test_type == PIPE_TEST)
		return test_pipe_cases(want_nodes, script, tokens);
}
