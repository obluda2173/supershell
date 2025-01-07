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

#include "parser.h"
#include "libft.h"
#include <unistd.h>


t_script_node *parse_pipe(t_dllist *tokens) {
	t_script_node* sn = (t_script_node*)malloc(sizeof(t_script_node));
	sn->node_type = PIPE_NODE;
	sn->num_children=2;

	while (tokens->prev)
		tokens = tokens->prev;

	sn->child1 = get_cmd_node(*(t_token*)tokens->content);
	if (!sn->child1) {
		free_script_node(sn);
		return get_error_node("no tokens");
	}
	fill_cmd_node(sn->child1, tokens);

	while (((t_token*)tokens->content)->type != PIPE)
		tokens = tokens->next;
	tokens = tokens->next;

	sn->child2 = get_cmd_node(*(t_token*)tokens->content);
	if (!sn->child2) {
		free(sn->child1);
		free(sn);
		return get_error_node( "no tokens");
	}
	fill_cmd_node(sn->child2, tokens);
	return sn;
}

t_script_node	*parse(t_dllist *tokens)
{
	if (!tokens)
		return get_error_node("no tokens");

	while (tokens->next) {
		if (((t_token*)tokens->content)->type == PIPE)
			return parse_pipe(tokens);
		tokens = tokens->next;
	}
	while (tokens->prev)
		tokens = tokens->prev;
	return (parse_cmd(tokens));
}
