/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:57/27 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/02 13:57:27 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include <unistd.h>

void	free_arguments(void *content)
{
	t_argument	*a;

	a = (t_argument *)content;
	free(a->word);
	free(a);
}

void	free_redirection(void *content)
{
	t_redirection	*r;

	r = (t_redirection *)content;
	free(r->word);
	free(r);
}

void	free_script_node(void *sn)
{
	t_script_node	*node;

	node = (t_script_node *)sn;
	if (node->node_type == PIPE_NODE || node->node_type == AND_NODE || node->node_type == OR_NODE) {
		if (node->upstream)
			free_script_node(node->upstream);
		if (node->downstream)
			free_script_node(node->downstream);
	}
	if (node->node_type == CMD_NODE)
	{
		if (node->node_data.cmd_node.arguments)
			ft_lstclear(&node->node_data.cmd_node.arguments, free_arguments);
		if (node->node_data.cmd_node.redirections)
			ft_lstclear(&node->node_data.cmd_node.redirections, free_redirection);
		free(node->node_data.cmd_node.cmd_token.content);
	}
	free(node);
}

t_token	copy_token(t_token token)
{
	t_token	copy;

	copy.content = ft_strdup(token.content);
	copy.type = token.type;
	return (copy);
}
