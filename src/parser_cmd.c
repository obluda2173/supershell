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

#include "libft.h"
#include "parser.h"
#include <unistd.h>

static t_script_node	*create_and_add_redirection(t_list **script,
		t_dllist *head, t_script_node *sn)
{
	t_redirection	*r;
	t_list			*tmp;

	r = extract_redirection(head);
	if (!r)
	{
		ft_lstclear(script, free_script_node);
		return (create_and_add_error_node(script, "parsing error redirection"));
	}
	tmp = ft_lstnew(r);
	if (!tmp)
	{
		free_redirection(r);
		ft_lstclear(script, free_script_node);
		return (NULL);
	}
	ft_lstadd_back(&sn->node_data.cmd_node.redirections, tmp);
	return (sn);
}

static t_script_node	*create_and_add_cmd_node(t_list **script,
		t_dllist *tokens)
{
	t_script_node	*sn;

	if (!tokens)
		return (create_and_add_error_node(script, "no tokens"));
	if ((*(t_token *)tokens->content).type == END_OF_FILE)
		return (NULL);
	sn = (t_script_node *)malloc(sizeof(t_script_node));
	if (!sn)
		return (NULL);
	init_cmd_node(sn, (*(t_token *)tokens->content));
	sn->node_type = CMD_NODE;
	*script = ft_lstnew(sn);
	if (!script)
	{
		free_script_node(sn);
		return (NULL);
	}
	return (sn);
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
	ft_lstadd_back(
		&((t_script_node *)((*script)->content))->node_data.cmd_node.arguments,
		tmp);
	return (tmp);
}

t_list	*fill_cmd_node(t_list *script, t_dllist *tokens)
{
	t_token			cur;
	t_script_node	*latest_node;

	while (tokens)
	{
		cur = *(t_token *)(tokens->content);
		if (cur.type == END_OF_FILE)
			return (script);
		if (cur.type == REDIRECT_OUT)
		{
			latest_node = create_and_add_redirection(&script, tokens,
					script->content);
			if (latest_node && latest_node->node_type == ERROR_NODE)
				return (script);
			tokens = tokens->next->next;
			continue ;
		}
		if (!create_and_add_argument(&script, tokens->content))
			return (NULL);
		tokens = tokens->next;
	}
	return (script);
}

t_list	*parse_cmd(t_list *script, t_dllist *tokens)
{
	t_script_node	*latest_node;

	latest_node = create_and_add_cmd_node(&script, tokens);
	if (latest_node && latest_node->node_type == ERROR_NODE)
		return (script);
	return (fill_cmd_node(script, tokens->next));
}
