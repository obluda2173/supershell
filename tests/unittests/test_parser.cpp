#include "test_main.hpp"

struct ParserTestParams {
	std::vector<t_token> token_vec;
	std::vector<t_test_script_node> sn_vec;
};

class ParserTestSuite : public::testing::TestWithParam<ParserTestParams>{};

void compare_cmd_node(t_test_script_node want, t_cmd_node got) {
	EXPECT_STREQ(want.node.cmd_token.content, got.cmd_token.content);
	EXPECT_EQ(want.node.cmd_token.type, got.cmd_token.type);
	t_list* got_arguments = got.arguments;
	int got_arg_count = ft_lstsize(got_arguments);
	EXPECT_EQ(want.node.argument_count, got_arg_count);
	if (want.node.argument_count == 0)
		EXPECT_EQ(NULL, got_arguments);

	t_list* head = got_arguments;
	for (int j = 0; j < want.node.argument_count; j++) {
		t_argument *got_arg = (t_argument*)head->content;
		EXPECT_STREQ(want.node.arguments[j].literal, got_arg->literal);
		EXPECT_EQ(want.node.arguments[j].type, got_arg->type);
		head = head->next;
	}
	if (want.node.redirects.size() > 0) {
		ASSERT_NE(got.redirections, nullptr);
		t_redirection r = *(t_redirection*)got.redirections->content;
		EXPECT_EQ(OUT, r.type);
		EXPECT_STREQ("output1", r.file);
	}
}

TEST_P(ParserTestSuite, ParserTest) {
	ParserTestParams params = GetParam();
	std::vector<t_test_script_node> want_nodes = params.sn_vec;

	t_dllist *tokens = create_token_dllist(params.token_vec);

	if (params.token_vec.size() == 0)
		ASSERT_EQ(nullptr, tokens);

	t_list *script = parse(tokens);
	t_list *head = script;

	if (!tokens || params.sn_vec.size() == 0) {
		ASSERT_EQ(nullptr, script);
		ft_dllstclear(&tokens, free_token);
		return;
	}

	ASSERT_NE(script, nullptr);
	for (t_test_script_node want_node : want_nodes) {
		EXPECT_EQ(CMD_NODE, (*(t_script_node*)head->content).node_type);
		compare_cmd_node(want_node, (*(t_script_node*)head->content).node);
		head=head->next;
	}

	ASSERT_EQ(head, nullptr);
	ft_dllstclear(&tokens, free_token);
	ft_lstclear(&script, free_script_node);
}

INSTANTIATE_TEST_SUITE_P(
	ParserTests, ParserTestSuite,
	testing::Values(
		ParserTestParams{{}, {}},
		ParserTestParams{{new_token(NULL, END_OF_FILE)}, {}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE,
								  new_test_cmd_node(new_token("echo", BUILTIN),
													{}, 0, {}))}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("file.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE,
								  new_test_cmd_node(new_token("echo", BUILTIN),
													{new_argument("file.txt", LITERAL),},
													1, {}))}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("file1.txt", WORD),
				new_token("file2.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN),
															  {new_argument("file1.txt", LITERAL),
															   new_argument("file2.txt", LITERAL)},
															  2, {}))}},
		ParserTestParams{{
				new_token("wc", WORD),
				new_token("-l", WORD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("wc", WORD),
																		   {
																			   new_argument("-l", LITERAL),
																		   },
																		   1, {}))}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("PATH", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN),
															  {
																  new_argument("PATH", ENV_EXP),
															  }, 1, {}))}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("?", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN),
															  {
																  new_argument("?", EXIT_STATUS_EXP),
															  },
															  1, {}))}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN),
															  {
																  new_argument("", ENV_EXP),
															  },
															  1, {}))}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token("some_*_file.c", WILDCARD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(CMD_NODE,
											   new_test_cmd_node(new_token("echo", BUILTIN),
																 {
																	 new_argument("some_*_file.c", WILDCARD_EXP),
																 },
																 1, {}))}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("this is a double quoted string", DOUBLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE,
								  new_test_cmd_node(
									  new_token("echo", BUILTIN),
									  {
										  new_argument("this is a double quoted string", LITERAL),
									  },
									  1, {}))}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token("string1 $PATH string2", SINGLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(CMD_NODE,
											   new_test_cmd_node(
												   new_token("echo", BUILTIN),
												   {
													   new_argument("string1 $PATH string2", LITERAL),
												   },
												   1, {}))}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token("string1 ", DOUBLE_QUOTE),
				new_token("PATH", DOLLAR),
				new_token(" string2", DOUBLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(CMD_NODE,
											   new_test_cmd_node(
												   new_token("echo", BUILTIN),
												   {
													   new_argument("string1 ", LITERAL),
													   new_argument("PATH", ENV_EXP),
													   new_argument(" string2", LITERAL),
												   }, 3, {}))}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token("string1 ", DOUBLE_QUOTE),
				new_token(">", REDIRECT_OUT),
				new_token("output1", WORD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(CMD_NODE,
											   new_test_cmd_node(
												   new_token("echo", BUILTIN),
												   {
													   new_argument("string1 ", LITERAL),
												   }, 1, {new_redirection("output1", OUT)}))}}

		));
