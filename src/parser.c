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

#include "libft.h"
#include "parser.h"
#include <unistd.h>

t_script_node	*parse_pipe(t_dllist *tokens)
{
	t_script_node	*sn;

	sn = (t_script_node *)malloc(sizeof(t_script_node));
	sn->node_type = PIPE_NODE;
	sn->num_children = 2;

	sn->downstream = parse_cmd(tokens->next);
	if (!sn->downstream)
	{
		free(sn);
		return (get_error_node("could not alloc space for pipe downstream"));
	}

	while (tokens->prev)
		tokens = tokens->prev;
	
	sn->upstream = parse_cmd(tokens);
	if (!sn->upstream)
	{
		free_script_node(sn);
		free(sn->downstream);
		return (get_error_node("could not alloc space for pipe upstream"));
	}

	return (sn);
}

t_script_node	*parse(t_dllist *tokens)
{
	if (!tokens)
		return (get_error_node("no tokens"));

	tokens = ft_dllstlast(tokens);
	while (tokens->prev)
	{
		if (((t_token *)tokens->content)->type == PIPE)
			return (parse_pipe(tokens));
		tokens = tokens->prev;
	}
	while (tokens->prev)
		tokens = tokens->prev;
	return (parse_cmd(tokens));
}
