/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:36/14 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/03 09:36:14 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>

t_redirection	*extract_redirection(t_dllist *tokens)
{
	t_redirection	*r;

	r = parse_redirection_token(*(t_token *)tokens->content);
	if (!r)
		return (NULL);
	return (parse_redirection_word(*(t_token *)(tokens->next->content), r));
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
	if (t->type == DOUBLE_QUOTE)
		arg->type = DOUBLE_QUOTE_STR;
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

static t_script_node	*create_and_add_redirection(t_dllist *head,
		t_script_node *sn)
{
	t_redirection	*r;
	t_list			*tmp;

	r = extract_redirection(head);
	if (!r)
	{
		free_script_node(sn);
		return (get_error_node("parsing error redirection"));
	}
	tmp = ft_lstnew(r);
	if (!tmp)
	{
		free_redirection(r);
		free_script_node(sn);
		return (NULL);
	}
	ft_lstadd_back(&sn->node_data.cmd_node.redirections, tmp);
	return (sn);
}

t_script_node	*create_and_add_argument(t_script_node *sn, t_token *t)
{
	t_argument	*arg;
	t_list		*tmp;

	arg = extract_argument(t);
	if (!arg)
	{
		free_script_node(sn);
		return (NULL);
	}
	tmp = ft_lstnew(arg);
	if (!tmp)
	{
		free_script_node(sn);
		return (NULL);
	}
	ft_lstadd_back(&sn->node_data.cmd_node.arguments, tmp);
	return (sn);
}

t_script_node	*fill_cmd_node(t_script_node *sn, t_dllist *tokens)
{
	t_script_node	*latest_node;

	if (!is_redirection_token(*(t_token *)(tokens->content)))
		tokens = tokens->next;
	while (tokens)
	{
		if (((t_token *)tokens->content)->type == PIPE)
			return (sn);
		if (((t_token *)tokens->content)->type == END_OF_FILE)
			return (sn);
		if (is_redirection_token(*(t_token *)tokens->content))
		{
			latest_node = create_and_add_redirection(tokens, sn);
			if (latest_node && latest_node->node_type == ERROR_NODE)
				return (latest_node);
			tokens = tokens->next->next;
			continue ;
		}
		if (!create_and_add_argument(sn, tokens->content))
			return (NULL);
		tokens = tokens->next;
	}
	return (sn);
}
