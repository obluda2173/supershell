#include "test_main.hpp"

t_script_node *copy_script_node(t_script_node sn) {
	t_script_node *copy = (t_script_node*)malloc(sizeof(t_script_node));
	copy->token = copy_token(*sn.token);
	return copy;
}

t_dllist *create_token_dllist(std::vector<t_token> tokens) {
	t_dllist *token_dllist = ft_dllstnew(copy_token(tokens[0]));;
	ft_dllstadd_back(&token_dllist, ft_dllstnew(copy_token(tokens[0])));
	return token_dllist;
}

t_list *create_script_list(std::vector<t_script_node> sn_vec) {
	t_list *script = ft_lstnew(copy_script_node(sn_vec[0]));;
	// ft_lstadd_back(&script, ft_lstnew(copy_script_node(sn_vec[1])));
	return script;
}

t_script_node new_script_node(t_token token) {
	return (t_script_node){&token};
}

void free_script_node(void *sn) {
	t_script_node *node;
	node = (t_script_node*)sn;
	free_token(node->token);
	free(node);
}

struct ParserTestParams {
	std::vector<t_token> token_vec;
	std::vector<t_script_node> sn_vec;
};

class ParserTestSuite : public::testing::TestWithParam<ParserTestParams>{};

TEST_P(ParserTestSuite, ParserTest) {
	ParserTestParams params = GetParam();

	t_dllist *tokens = create_token_dllist(params.token_vec);
	t_list *script = parse(tokens);

	ASSERT_NE(script, nullptr);
	t_list *head = script;
	t_script_node cur_node;
	cur_node = *(t_script_node*)head->content;
	EXPECT_STREQ(cur_node.token->content, "echo");
	EXPECT_EQ(cur_node.token->type, BUILTIN);

	ft_dllstclear(&tokens, free_token);
	ft_lstclear(&script, free_script_node);
}


INSTANTIATE_TEST_SUITE_P(ParserTests, ParserTestSuite,
						 testing::Values(ParserTestParams{
								 {
									 new_token("echo", BUILTIN),
									 new_token(NULL, END_OF_FILE),
								 },
								 {
									 new_script_node(new_token("echo", BUILTIN))
								 }
							 }));
