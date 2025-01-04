/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:01/54 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/02 14:01:54 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	init_cmd_node(t_script_node *sn, t_token t)
{
	sn->node_data.cmd_node.cmd_token = copy_token(t);
	sn->node_data.cmd_node.arguments = NULL;
	sn->node_data.cmd_node.redirections = NULL;
}

t_redirection	*extract_redirection(t_dllist *tokens)
{
	t_token			cur;
	t_redirection	*r;

	cur = *(t_token *)(tokens->content);
	r = (t_redirection *)malloc(sizeof(t_redirection));
	if (!r)
		return (NULL);
	r->type = OUT;
	tokens = tokens->next;
	cur = *(t_token *)(tokens->content);
	if (cur.type != WORD)
	{
		free(r);
		return (NULL);
	}
	r->file = ft_strdup(cur.content);
	r->word = ft_strdup(cur.content);
	r->word_type = LITERAL;
	return (r);
}

t_argument	*extract_argument(t_token *t)
{
	t_argument	*arg;

	arg = (t_argument *)malloc(sizeof(t_argument));
	if (!arg)
		return (NULL);
	arg->word = ft_strdup(t->content);
	if (t->type == WORD || t->type == DOUBLE_QUOTE || t->type == SINGLE_QUOTE)
		arg->type = LITERAL;
	if (t->type == DOLLAR)
	{
		arg->type = ENV_EXP;
		if (!ft_strncmp(arg->word, "?", 1))
			arg->type = EXIT_STATUS_EXP;
	}
	if (t->type == WILDCARD)
		arg->type = WILDCARD_EXP;
	return (arg);
}
