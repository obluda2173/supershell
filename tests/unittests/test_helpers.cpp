#include "test_main.hpp"

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

t_token new_token(const char* content, t_token_type type) {
	return (t_token){(char*)content, type};
}

t_test_cmd_node new_test_cmd_node(t_token cmd_token, std::vector<t_argument> args,  std::vector<t_redirection> redirects) {
	return (t_test_cmd_node){cmd_token, args,  redirects};
}

t_test_script_node new_test_script_node(t_node_type type, t_test_cmd_node cn, t_error_node en, std::vector<t_test_script_node> pipe_node_childs) {
	return (t_test_script_node){type, cn, en, pipe_node_childs};
}

t_argument new_argument(const char* literal, t_word_type type) {
	return (t_argument){(char*)literal, type};
}

t_argument *new_argument_pointer(const char* literal, t_word_type type) {
	t_argument *a = (t_argument*)malloc(sizeof(t_argument));
	if (!a)
		exit(EXIT_FAILURE);
	a->type = type;
	a->word = ft_strdup(literal);
	return a;
}

t_redirection new_redirection(int fd, t_redirection_type type, const char* word, t_word_type wt) {
	return (t_redirection){fd, type, (char*)word, wt};
}

t_error_node new_error_node(const char* error) {
	return {error};
}

