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

static t_script_node	*create_and_add_redirection(t_dllist *head, t_script_node *sn)
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
		/* ft_lstclear(script, free_script_node); */
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
	return sn;
}

t_script_node	*fill_cmd_node(t_script_node *sn, t_dllist *tokens)
{
	t_token			cur;
	t_script_node	*latest_node;

	cur = *(t_token *)(tokens->content);
	if (cur.type != REDIRECT_OUT && cur.type != REDIRECT_IN && cur.type != REDIRECT_APPEND && cur.type != HERE_DOC)
		tokens = tokens->next;
	while (tokens)
	{
		cur = *(t_token *)(tokens->content);
		if (cur.type == PIPE || cur.type == AND)
			return sn;
		if (cur.type == END_OF_FILE)
			return sn;
		if (cur.type == REDIRECT_OUT || cur.type == REDIRECT_IN ||
			cur.type == REDIRECT_APPEND || cur.type == HERE_DOC )
		{
			latest_node = create_and_add_redirection( tokens, sn);
			if (latest_node && latest_node->node_type == ERROR_NODE)
				return latest_node;
			tokens = tokens->next->next;
			continue ;
		}
		if (!create_and_add_argument(sn, tokens->content))
			return (NULL);
		tokens = tokens->next;
	}
	return sn;
}

t_script_node	*parse_cmd(t_dllist *tokens)
{
	t_script_node	*sn;

	if ((*(t_token *)tokens->content).type == END_OF_FILE)
		return (NULL);
	sn = get_cmd_node(*(t_token *)tokens->content);
	if (!sn)
		return (NULL);
	return (fill_cmd_node(sn, tokens));
}
