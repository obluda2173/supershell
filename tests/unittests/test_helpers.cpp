#include "test_main.hpp"

t_token new_token(const char* content, token_type type) {
	return (t_token){(char*)content, type};
}

t_dllist *create_token_dllist(std::vector<t_token> tokens) {
	t_token *t;
	t_dllist *token_dllist = NULL;

	for (size_t i = 0; i < tokens.size() ; i++) {
		t = (t_token*)malloc(sizeof(t_token));
		if (tokens[i].content)
			t->content = ft_strdup(tokens[i].content);
		else
			t->content = NULL;
		t->type = tokens[i].type;
		ft_dllstadd_back(&token_dllist, ft_dllstnew(t));
	}
	return token_dllist;
}

t_test_cmd_node new_test_cmd_node(t_token cmd_token, std::vector<t_argument> args,  std::vector<t_redirection> redirects) {
	return (t_test_cmd_node){cmd_token, args,  redirects};
}

t_test_script_node new_test_script_node(t_node_type type, t_test_cmd_node cn, t_error_node en) {
	return (t_test_script_node){type, cn, en};
}

t_argument new_argument(const char* literal, t_word_type type) {
	return (t_argument){(char*)literal, type};
}

t_redirection new_redirection(int fd, t_redirection_type type, const char* word, t_word_type wt) {
	return (t_redirection){fd, type, (char*)word, wt};
}

t_error_node new_error_node(const char* error) {
	return {error};
}

void compare_redirections(std::vector<t_redirection> want, t_list *got) {

	if (want.size() == 0) {
		ASSERT_EQ(got, nullptr);
		return;
	}
	ASSERT_NE(got, nullptr);
	t_redirection r = *(t_redirection*)got->content;
	EXPECT_EQ(want[0].fd, r.fd);
	EXPECT_EQ(want[0].type, r.type);
	EXPECT_STREQ(want[0].word, r.word);
	EXPECT_EQ(OUT, r.word_type);
}

void compare_arguments(std::vector<t_argument> want, t_list* got) {
	int got_arg_count = ft_lstsize(got);
	EXPECT_EQ(want.size(), got_arg_count);
	if (want.size() == 0)
		EXPECT_EQ(NULL, got);

	t_list* head = got;
	for (size_t j = 0; j < want.size(); j++) {
		t_argument *got_arg = (t_argument*)head->content;
		EXPECT_STREQ(want[j].word, got_arg->word);
		EXPECT_EQ(want[j].type, got_arg->type);
		head = head->next;
	}
}

void compare_cmd_node(t_test_script_node want, t_cmd_node got) {
	EXPECT_STREQ(want.cmd_node.cmd_token.content, got.cmd_token.content);
	EXPECT_EQ(want.cmd_node.cmd_token.type, got.cmd_token.type);
	compare_arguments(want.cmd_node.arguments, got.arguments);
	compare_redirections(want.cmd_node.redirects, got.redirections);
}
