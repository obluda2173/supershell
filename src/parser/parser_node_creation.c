/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_node_creation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 09:39/17 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/09 09:39:17 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_script_node	*fill_cmd_node(t_script_node *sn, t_dllist *tokens)
{
	t_script_node	*latest_node;
	t_token			cur_token;

	while (tokens)
	{
		cur_token = *((t_token *)tokens->content);
		if (cur_token.type == LPAREN)
			return (teardown(sn, "parsing error near ("));
		if (cur_token.type == PIPE || cur_token.type == END_OF_FILE)
			return (sn);
		if (sn->node_data.cmd_node.cmd_token.type == NONE
			&& (cur_token.type == BUILTIN || cur_token.type == WORD))
			sn->node_data.cmd_node.cmd_token = copy_token(cur_token);
		else if (is_redirection_token(*(t_token *)tokens->content))
		{
			latest_node = create_and_add_redirection(tokens, sn);
			if (latest_node && latest_node->node_type == ERROR_NODE)
				return (latest_node);
			tokens = tokens->next;
		}
		else if (!create_and_add_argument(sn, tokens->content))
			return (NULL);
		tokens = tokens->next;
	}
	return (sn);
}

t_script_node	*get_cmd_node(void)
{
	t_script_node	*sn;

	sn = (t_script_node *)malloc(sizeof(t_script_node));
	if (!sn)
		return (NULL);
	sn->node_type = CMD_NODE;
	sn->node_data.cmd_node.arguments = NULL;
	sn->node_data.cmd_node.redirections = NULL;
	sn->node_data.cmd_node.cmd_token.type = NONE;
	sn->node_data.cmd_node.cmd_token.content = NULL;
	sn->num_children = 0;
	sn->upstream = NULL;
	sn->downstream = NULL;
	return (sn);
}

t_script_node	*get_error_node(const char *error)
{
	t_script_node	*sn;

	sn = (t_script_node *)malloc(sizeof(t_script_node));
	if (!sn)
		return (NULL);
	sn->node_type = ERROR_NODE;
	sn->node_data.error_node.error = error;
	return (sn);
}

t_script_node	*get_branch_node(t_node_type nt)
{
	t_script_node	*sn;

	sn = (t_script_node *)malloc(sizeof(t_script_node));
	if (!sn)
		return (NULL);
	sn->node_type = nt;
	sn->num_children = 2;
	sn->downstream = NULL;
	sn->upstream = NULL;
	return (sn);
}
