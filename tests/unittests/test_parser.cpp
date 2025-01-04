#include "test_main.hpp"

struct ParserTestParams {
	std::vector<t_token> token_vec;
	std::vector<t_test_script_node> want_nodes;
};

class ParserTestSuite : public::testing::TestWithParam<ParserTestParams>{};

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
		ASSERT_NE(nullptr, script);
		ASSERT_EQ(ERROR_NODE, ((t_script_node*)script->content)->node_type);
		ASSERT_STREQ(want_nodes[0].err_node.error, ((t_script_node*)script->content)->node_data.error_node.error);
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

INSTANTIATE_TEST_SUITE_P(
	ParserTestsErrorHandling,
	ParserTestSuite,
	testing::Values(
		ParserTestParams{{
				new_token(">>", REDIRECT_APPEND),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token("<", REDIRECT_IN),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token("cat", BUILTIN),
				new_token("<", REDIRECT_IN),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token(">", REDIRECT_OUT),
				new_token(NULL, END_OF_FILE),

			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token(">", REDIRECT_OUT),
				new_token(NULL, END_OF_FILE),
			}, {new_test_script_node(ERROR_NODE, {}, new_error_node("parsing error redirection"))}},
		ParserTestParams{{}, {new_test_script_node(ERROR_NODE, {}, new_error_node("no tokens"))}}
		)
	);

INSTANTIATE_TEST_SUITE_P(
	ParserTestsCmdsWithRedirections,
	ParserTestSuite,
	testing::Values(
		// ParserTestParams{
		// 	{
		// 		new_token("<<", HERE_DOC),
		// 		new_token("line1\n$PATH\nline2\n", WORD),
		// 		new_token(NULL, END_OF_FILE),
		// 	},
		// 	{new_test_script_node(
		// 			CMD_NODE,
		// 			{new_test_cmd_node(new_token("<<", HERE_DOC), {},
		// 							   {new_redirection("line1\n$PATH\nline2\n", HERED)})},
		// 			{})}},
		// ParserTestParams{
		// 	{
		// 		new_token("<<", HERE_DOC),
		// 		new_token("line1\nline2\n", WORD),
		// 		new_token(NULL, END_OF_FILE),
		// 	},
		// 	{new_test_script_node(
		// 			CMD_NODE,
		// 			{new_test_cmd_node(new_token("<<", HERE_DOC), {},
		// 							   {new_redirection("line1\nline2\n", HERED)})},
					// {})}},
		ParserTestParams{
			{
				new_token(">>", REDIRECT_APPEND),
				new_token("test.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					{new_test_cmd_node(new_token(">>", REDIRECT_APPEND), {},
									   {new_redirection("test.txt", APPEND, "test.txt", LITERAL)})},
					{})}},
		ParserTestParams{{
				new_token("cat", WORD),
				new_token("<", REDIRECT_IN),
				new_token("input1", WORD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(CMD_NODE,
											   new_test_cmd_node(
												   new_token("cat", WORD),
												   {},  {new_redirection("input1", IN, "input1", LITERAL)}), {})}},
		ParserTestParams{
			{
				new_token("<", REDIRECT_IN),
				new_token("test.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					{new_test_cmd_node(new_token("<", REDIRECT_IN), {},
									   {new_redirection("test.txt", IN, "test.txt", LITERAL)})},
					{})}},
		ParserTestParams{
			{
				new_token(">", REDIRECT_OUT),
				new_token("test.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					{new_test_cmd_node(new_token(">", REDIRECT_OUT), {},
									   {new_redirection("test.txt", OUT, "test.txt", LITERAL)})},
					{})}},
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
												   },  {new_redirection("output1", OUT, "output1", LITERAL)}), {})}}

		)
	);

INSTANTIATE_TEST_SUITE_P(
	ParserTestsCmdsWithoutRedirections, ParserTestSuite,
	testing::Values(
		ParserTestParams{{new_token(NULL, END_OF_FILE)}, {}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE, new_test_cmd_node(new_token("echo", BUILTIN), {}, {}),
					{})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("file.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE,
								  new_test_cmd_node(new_token("echo", BUILTIN),
													{
														new_argument("file.txt",
																	 LITERAL),
													},
													{}),
								  {})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("file1.txt", WORD),
				new_token("file2.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(new_token("echo", BUILTIN),
									  {new_argument("file1.txt", LITERAL),
									   new_argument("file2.txt", LITERAL)},
									  {}),
					{})}},
		ParserTestParams{{
				new_token("wc", WORD),
				new_token("-l", WORD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(
								 CMD_NODE,
								 new_test_cmd_node(new_token("wc", WORD),
												   {
													   new_argument("-l", LITERAL),
												   },
												   {}),
								 {})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("PATH", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(CMD_NODE,
								  new_test_cmd_node(new_token("echo", BUILTIN),
													{
														new_argument("PATH",
																	 ENV_EXP),
													},
													{}),
								  {})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("?", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(new_token("echo", BUILTIN),
									  {
										  new_argument("?", EXIT_STATUS_EXP),
									  },
									  {}),
					{})}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token("", DOLLAR),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(
								 CMD_NODE,
								 new_test_cmd_node(new_token("echo", BUILTIN),
												   {
													   new_argument("", ENV_EXP),
												   },
												   {}),
								 {})}},
		ParserTestParams{{
				new_token("echo", BUILTIN),
				new_token("some_*_file.c", WILDCARD),
				new_token(NULL, END_OF_FILE),
			},
						 {new_test_script_node(
								 CMD_NODE,
								 new_test_cmd_node(new_token("echo", BUILTIN),
												   {
													   new_argument("some_*_file.c",
																	WILDCARD_EXP),
												   },
												   {}),
								 {})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("this is a double quoted string", DOUBLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(
						new_token("echo", BUILTIN),
						{
							new_argument("this is a double quoted string", LITERAL),
						},
						{}),
					{})}},
		ParserTestParams{
			{
				new_token("echo", BUILTIN),
				new_token("string1 $PATH string2", SINGLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
			{new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(new_token("echo", BUILTIN),
									  {
										  new_argument("string1 $PATH string2",
													   LITERAL),
									  },
									  {}),
					{})}},
		ParserTestParams {
			{
				new_token("echo", BUILTIN),   new_token("string1 ", DOUBLE_QUOTE),
				new_token("PATH", DOLLAR),    new_token(" string2", DOUBLE_QUOTE),
				new_token(NULL, END_OF_FILE),
			},
			{
				new_test_script_node(
					CMD_NODE,
					new_test_cmd_node(new_token("echo", BUILTIN),
									  {
										  new_argument("string1 ", LITERAL),
										  new_argument("PATH", ENV_EXP),
										  new_argument(" string2", LITERAL),
									  },
									  {}),
					{})}}
		));
