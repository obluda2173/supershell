#include "test_main.hpp"

t_dllist *create_token_dllist(std::vector<t_token> tokens) {
	t_token* t0 = (t_token*)malloc(sizeof(t_token));
	t0->content = ft_strdup(tokens[0].content);
	t0->type = tokens[0].type;
	t_dllist *token_dllist = ft_dllstnew(t0);;

	t_token* t1 = (t_token*)malloc(sizeof(t_token));
	if (tokens[1].content)
		t1->content = ft_strdup(tokens[1].content);
	else
		t1->content = NULL;
	t1->type = tokens[1].type;
	ft_dllstadd_back(&token_dllist, ft_dllstnew(t1));
	return token_dllist;
}

t_script_node new_script_node(t_token token) {
	return (t_script_node){token};
}

void free_script_node(void *sn) {
	t_script_node *node;
	node = (t_script_node*)sn;
	free(node->token.content);
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
	t_script_node want_node = params.sn_vec[0];
	EXPECT_STREQ(want_node.token.content, cur_node.token.content);
	EXPECT_EQ(want_node.token.type, cur_node.token.type);

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
