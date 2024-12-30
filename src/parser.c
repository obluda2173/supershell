#include "parser.h"
#include "lexer.h"
#include "libft.h"

t_token copy_token(t_token token) {
	t_token copy;
	copy.content = ft_strdup(token.content);
	copy.type = token.type;
	return copy;
}


t_list *parse(t_dllist *tokens) {
	t_script_node *sn = (t_script_node*)malloc(sizeof(t_script_node));
	if (!sn)
		return NULL;

	t_dllist *head = tokens;
	t_list *script = NULL;

	sn->token = copy_token(*(t_token*)head->content);
	sn->type = CMD_NODE;
	sn->arguments= NULL;
	sn->argument_count = 0;
	script = ft_lstnew(sn);

	head = head->next;

	while (head) {
		t_token* cur = (t_token*)(head->content);
		if (cur->type == END_OF_FILE)
			return script;
		sn->arguments = (t_argument**)malloc(sizeof(t_argument*));
		sn->arguments[0] = (t_argument*)malloc(sizeof(t_argument));
		sn->arguments[0]->literal = ft_strdup(cur->content);
		sn->argument_count = 1;
		head = head->next;
	}

	return script;
}
