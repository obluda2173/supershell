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
