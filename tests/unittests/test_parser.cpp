#include "test_main.hpp"

struct ParserTestParams {
	std::vector<t_token> token_vec;
	std::vector<t_test_script_node> sn_vec;
};

class ParserTestSuite : public::testing::TestWithParam<ParserTestParams>{};

void compare_script_node(t_test_script_node want, t_script_node got) {
	EXPECT_STREQ(want.token.content, got.token.content);
	EXPECT_EQ(want.token.type, got.token.type);
	EXPECT_EQ(want.type, got.type);
	EXPECT_EQ(want.argument_count, got.argument_count);
	if (want.argument_count == 0)
		EXPECT_EQ(NULL,got.arguments);
	for (int j = 0; j < want.argument_count; j++)
		EXPECT_STREQ(want.arguments[j].literal, got.arguments[j]->literal);
}

TEST_P(ParserTestSuite, ParserTest) {
	ParserTestParams params = GetParam();
	std::vector<t_test_script_node> want_nodes = params.sn_vec;

	t_dllist *tokens = create_token_dllist(params.token_vec);
	t_list *script = parse(tokens);
	t_list *head = script;

	ASSERT_NE(script, nullptr);
	for (t_test_script_node want_node : want_nodes) {
		compare_script_node(want_node, *(t_script_node*)head->content);
		head=head->next;
	}
	ASSERT_EQ(head, nullptr);
	ft_dllstclear(&tokens, free_token);
	ft_lstclear(&script, free_script_node);
}

t_argument new_argument(const char* literal) {
	return (t_argument){(char*)literal};
}

INSTANTIATE_TEST_SUITE_P(
	ParserTests,
	ParserTestSuite,
	testing::Values(
		ParserTestParams {
			{
				new_token("echo", BUILTIN),
				new_token(NULL, END_OF_FILE),
			},
			{
				new_test_script_node(new_token("echo", BUILTIN), CMD_NODE, {}, 0)
			}
		},
		ParserTestParams {
			{
				new_token("echo", BUILTIN),
				new_token("file.txt", WORD),
				new_token(NULL, END_OF_FILE),
			},
			{
				new_test_script_node(new_token("echo", BUILTIN), CMD_NODE, {new_argument("file.txt")}, 1)
			}
		}
		));
