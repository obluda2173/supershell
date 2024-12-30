#include "parser.h"
#include "lexer.h"
#include "libft.h"

void free_script_node(void *sn) {
	t_script_node *node;
	node = (t_script_node*)sn;

	for (int i = 0; i < node->argument_count; i++) {
		free(node->arguments[i]->literal);
		free(node->arguments[i]);
	}
	free(node->arguments);
	free(node->token.content);
	free(node);
}

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

	t_token cur = (*(t_token*)head->content);
	sn->token = copy_token(cur);
	sn->type = CMD_NODE;
	script = ft_lstnew(sn);

	head = head->next;
	sn->argument_count = ft_dllstsize(head) - 1;
	if (sn->argument_count == 0)
		sn->arguments = NULL;
	else
		sn->arguments = (t_argument**)malloc(sizeof(t_argument*) * sn->argument_count);

	int count = 0;
	while (head) {
		cur = *(t_token*)(head->content);
		if (cur.type == END_OF_FILE)
			return script;
		sn->arguments[count] = (t_argument*)malloc(sizeof(t_argument));
		sn->arguments[count]->literal = ft_strdup(cur.content);
		head = head->next;
		count++;
	}

	return script;
}
