#include "test_main.hpp"

t_token new_token(const char* content, token_type type) {
	return (t_token){(char*)content, type};
}

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

t_test_script_node new_test_script_node(t_token token, t_node_type type, std::vector<std::string> args, int argument_count) {
	return (t_test_script_node){token, type, args, argument_count};
}

void free_script_node(void *sn) {
	t_script_node *node;
	node = (t_script_node*)sn;

	for (int i = 0; i < node->argument_count; i++) {
		free(node->arguments[i]);
	}
	free(node->arguments);
	free(node->token.content);
	free(node);
}
