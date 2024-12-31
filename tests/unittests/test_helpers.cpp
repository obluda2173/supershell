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

t_test_script_node new_test_script_node(t_token token, t_node_type type, std::vector<t_argument> args, int argument_count) {
	return (t_test_script_node){token, type, args, argument_count};
}

t_argument new_argument(const char* literal, t_argument_type type) {
	return (t_argument){(char*)literal, type};
}
