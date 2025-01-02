#include "parser.h"
#include "lexer.h"
#include "libft.h"

void	free_arguments(void *content)
{
	t_argument *a;
	a = (t_argument *)content;
	free(a->literal);
	free(a);
}

void	free_redirection(void *content)
{
	t_redirection *r;
	r = (t_redirection *)content;
	free(r->file);
	free(r);
}

void free_script_node(void *sn) {
	t_script_node *node;
	node = (t_script_node*)sn;
	ft_lstclear(&node->arguments, free_arguments);
	ft_lstclear(&node->redirections, free_redirection);
	free(node->cmd_token.content);
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
	sn->cmd_token = copy_token(cur);
	sn->type = CMD_NODE;
	sn->arguments= NULL;
	sn->redirections= NULL;
	script = ft_lstnew(sn);

	head = head->next;

	t_dllist *head2 = head;
	cur = *(t_token *)(head2->content);
	while ((cur.type != END_OF_FILE) && (cur.type != REDIRECT_OUT)) {
		head2 = head2->next;
		cur = *(t_token *)(head2->content);
	}

	while (head) {
		cur = *(t_token*)(head->content);
		if (cur.type == REDIRECT_OUT) {
			t_redirection *r = (t_redirection*)malloc(sizeof(t_redirection));
			r->type = OUT;
			head = head->next;
			cur = *(t_token*)(head->content);
			r->file = ft_strdup(cur.content);
			ft_lstadd_back(&sn->redirections, ft_lstnew(r));
			head = head->next;
			continue;
		}
		if (cur.type == END_OF_FILE)
			return script;
		t_argument *arg =  (t_argument*)malloc(sizeof(t_argument));
		arg->literal = ft_strdup(cur.content);
		if (cur.type == WORD || cur.type == DOUBLE_QUOTE || cur.type == SINGLE_QUOTE)
			arg->type = LITERAL;
		if (cur.type == DOLLAR) {
			arg->type = ENV_EXP;
			if (!ft_strncmp(arg->literal, "?", 1))
				arg->type = EXIT_STATUS_EXP;
		}
		if (cur.type == WILDCARD)
			arg->type = WILDCARD_EXP;
		ft_lstadd_back(&sn->arguments, ft_lstnew(arg));

		head = head->next;
	}

	return script;
}
