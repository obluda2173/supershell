#include "test_main.hpp"

TEST(ParserTestSuite, ParserTestsLogicalOperators) {
	std::vector<t_token> token_vec = {
		new_token("echo", BUILTIN),
		new_token("hello", WORD),
		new_token("&&", AND),
		new_token("ls", WORD),
		new_token("-a", WORD),
		new_token(NULL, END_OF_FILE)
	} ;
	t_dllist *tokens = create_token_dllist(token_vec);
	if (token_vec.size() == 0)
		ASSERT_EQ(nullptr, tokens);

	t_test_script_node want = new_test_script_node(
		LOGICAL_NODE,
		{},
		{},
		{
			new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {new_argument("hello", LITERAL)}, {}), {}, {}),
			new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("ls", WORD), {new_argument("-a", LITERAL)}, {}), {}, {})
		});

	t_script_node *sn = parse(tokens);
	if (token_vec.size() == 1 && token_vec[0].type == END_OF_FILE) {
		ASSERT_EQ(nullptr, sn);
		ft_dllstclear(&tokens, free_token);
		return;
	}

	ASSERT_NE(nullptr, sn);
	ASSERT_EQ(sn->node_type, LOGICAL_NODE);
	ASSERT_EQ(sn->num_children, 2);
	ASSERT_NE(nullptr,sn->upstream);
	ASSERT_EQ(want.pipe_node_childs[0].type, sn->upstream->node_type);
	ASSERT_NE(nullptr,sn->downstream);
	ASSERT_EQ(want.pipe_node_childs[1].type, sn->downstream->node_type);
	free_script_node(sn);
	ft_dllstclear(&tokens, free_token);
}
