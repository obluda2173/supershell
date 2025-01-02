#include "lexer.h"
#include "libft.h"
#include "parser.h"

void	free_arguments(void *content)
{
	t_argument	*a;

	a = (t_argument *)content;
	free(a->literal);
	free(a);
}

void	free_redirection(void *content)
{
	t_redirection	*r;

	r = (t_redirection *)content;
	free(r->file);
	free(r);
}

void	free_script_node(void *sn)
{
	t_script_node	*node;

	node = (t_script_node *)sn;
	ft_lstclear(&node->arguments, free_arguments);
	ft_lstclear(&node->redirections, free_redirection);
	free(node->cmd_token.content);
	free(node);
}

t_token	copy_token(t_token token)
{
	t_token	copy;

	copy.content = ft_strdup(token.content);
	copy.type = token.type;
	return (copy);
}

void init_script_node(t_script_node *sn, t_token t) {
	sn->cmd_token = copy_token(t);
	sn->type = CMD_NODE;
	sn->arguments = NULL;
	sn->redirections = NULL;
}

t_redirection *extract_redirection(t_dllist *tokens) {
	t_token cur = *(t_token *)(tokens->content);
	t_redirection *r = (t_redirection *)malloc(sizeof(t_redirection));
	if (!r)
		return (NULL);
	r->type = OUT;
	tokens = tokens->next;
	cur = *(t_token *)(tokens->content);
	r->file = ft_strdup(cur.content);
	return r;
}

t_list	*parse(t_dllist *tokens)
{
	t_script_node	*sn;
	t_dllist		*head;
	t_list			*script;
	t_token			cur;
	t_redirection	*r;
	t_argument		*arg;
	t_list			*tmp;

	head = tokens;
	script = NULL;
	cur = (*(t_token *)head->content);

	sn = (t_script_node *)malloc(sizeof(t_script_node));
	if (!sn)
		return (NULL);
	init_script_node(sn, cur);

	script = ft_lstnew(sn);
	if (!script)
	{
		free_script_node(sn);
		return (NULL);
	}

	head = head->next;
	while (head)
	{
		cur = *(t_token *)(head->content);
		if (cur.type == REDIRECT_OUT)
		{
			r = extract_redirection(head);
			if (!r)
			{
				ft_lstclear(&script, free_script_node);
				return (NULL);
			}
			tmp = ft_lstnew(r);
			if (!tmp) {
				free_redirection(r);
				ft_lstclear(&script, free_script_node);
				return NULL;
			}
			ft_lstadd_back(&sn->redirections, tmp);
			head = head->next->next;
			continue ;
		}
		if (cur.type == END_OF_FILE)
			return (script);
		arg = (t_argument *)malloc(sizeof(t_argument));
		if (!arg)
		{
			ft_lstclear(&script, free_script_node);
			return (NULL);
		}
		arg->literal = ft_strdup(cur.content);
		if (cur.type == WORD || cur.type == DOUBLE_QUOTE
			|| cur.type == SINGLE_QUOTE)
			arg->type = LITERAL;
		if (cur.type == DOLLAR)
		{
			arg->type = ENV_EXP;
			if (!ft_strncmp(arg->literal, "?", 1))
				arg->type = EXIT_STATUS_EXP;
		}
		if (cur.type == WILDCARD)
			arg->type = WILDCARD_EXP;
		tmp = ft_lstnew(arg);
		if (!tmp)
		{
			ft_lstclear(&script, free_script_node);
			return (NULL);
		}
		ft_lstadd_back(&sn->arguments, tmp);
		head = head->next;
	}
	return (script);
}
