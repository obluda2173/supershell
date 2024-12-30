#include "test_main.hpp"
#include <gtest/gtest.h>

typedef struct s_script_node_test {
	t_token token;
	t_node_type type;
	std::vector<std::string> arguments;
	int argument_count;

} t_test_script_node;

t_test_script_node new_test_script_node(t_token token, t_node_type type, std::vector<std::string> args, int argument_count) {
	return (t_test_script_node){token, type, args, argument_count};
}

void free_script_node(void *sn) {
	t_script_node *node;
	node = (t_script_node*)sn;
	free(node->token.content);
	free(node);
}

struct ParserTestParams {
	std::vector<t_token> token_vec;
	std::vector<t_test_script_node> sn_vec;
};

class ParserTestSuite : public::testing::TestWithParam<ParserTestParams>{};

TEST_P(ParserTestSuite, ParserTest) {
	ParserTestParams params = GetParam();

	t_dllist *tokens = create_token_dllist(params.token_vec);
	t_list *script = parse(tokens);
	t_list *head = script;
	t_script_node cur_node;

	ASSERT_NE(script, nullptr);
	for (size_t i = 0; i < params.sn_vec.size() ; i++ ) {
		cur_node = *(t_script_node*)head->content;
		EXPECT_STREQ(params.sn_vec[i].token.content, cur_node.token.content);
		EXPECT_EQ(params.sn_vec[i].token.type, cur_node.token.type);
		EXPECT_EQ(params.sn_vec[i].type, cur_node.type);
		EXPECT_EQ(params.sn_vec[i].argument_count, cur_node.argument_count);
		for (int j = 0; j < params.sn_vec[i].argument_count; j++)
			EXPECT_STREQ(params.sn_vec[i].arguments[j].c_str(), cur_node.arguments[j]);
		head=head->next;
	}
	ASSERT_EQ(head, nullptr);
	ft_dllstclear(&tokens, free_token);
	ft_lstclear(&script, free_script_node);
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
				new_test_script_node(new_token("echo", BUILTIN), CMD_NODE, {""}, 0)
			}
		}
		// ParserTestParams {
		// 	{
		// 		new_token("echo", BUILTIN),
		// 		new_token(NULL, END_OF_FILE),
		// 	},
		// 	{
		// 		new_script_node(new_token("echo", BUILTIN))
		// 	}
		// }


		));
