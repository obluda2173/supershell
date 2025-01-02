/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:57/10 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/02 13:57:10 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"

static t_list	*create_and_add_redirection(t_list **script, t_dllist *head,
		t_script_node *sn)
{
	t_redirection	*r;
	t_list			*tmp;

	r = extract_redirection(head);
	if (!r)
	{
		ft_lstclear(script, free_script_node);
		return (NULL);
	}
	tmp = ft_lstnew(r);
	if (!tmp)
	{
		free_redirection(r);
		ft_lstclear(script, free_script_node);
		return (NULL);
	}
	ft_lstadd_back(&sn->node_data.cmd_node.redirections, tmp);
	return (tmp);
}

static t_list	*create_and_add_cmd_node(t_list **script, t_dllist *tokens)
{
	t_token			cur;
	t_script_node	*sn;

	cur = (*(t_token *)tokens->content);
	if (cur.type == END_OF_FILE)
		return NULL;
	sn = (t_script_node *)malloc(sizeof(t_script_node));
	if (!sn)
		return (NULL);
	init_cmd_node(sn, cur);
	sn->node_type = CMD_NODE;
	*script = ft_lstnew(sn);
	if (!script)
	{
		free_script_node(sn);
		return (NULL);
	}
	return (*script);
}

static t_list	*create_and_add_argument(t_list **script, t_token *t)
{
	t_argument	*arg;
	t_list		*tmp;

	arg = extract_argument(t);
	if (!arg)
	{
		ft_lstclear(script, free_script_node);
		return (NULL);
	}
	tmp = ft_lstnew(arg);
	if (!tmp)
	{
		ft_lstclear(script, free_script_node);
		return (NULL);
	}
	ft_lstadd_back(&((t_script_node *)((*script)->content))->node_data.cmd_node.arguments, tmp);
	return (tmp);
}

t_list	*parse(t_dllist *tokens)
{
	t_list	*script;
	t_token	cur;

	script = NULL;
	if (!tokens)
		return script;
	if (!create_and_add_cmd_node(&script, tokens))
		return (NULL);
	tokens = tokens->next;
	while (tokens)
	{
		cur = *(t_token *)(tokens->content);
		if (cur.type == END_OF_FILE)
			return (script);
		if (cur.type == REDIRECT_OUT)
		{
			if (!create_and_add_redirection(&script, tokens, script->content))
				return (NULL);
			tokens = tokens->next->next;
			continue ;
		}
		if (!create_and_add_argument(&script, tokens->content))
			return (NULL);
		tokens = tokens->next;
	}
	return (script);
}
