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
	sn->child1 = malloc(sizeof(t_script_node));
	if (!sn->child1) {
		free_script_node(sn);
		return create_and_add_error_node("no tokens");
	}
	sn->child2 = malloc(sizeof(t_script_node));
	if (!sn->child2) {
		free(sn->child1);
		free(sn);
		return create_and_add_error_node( "no tokens");
	}

	while (tokens->prev)
		tokens = tokens->prev;
	init_cmd_node(sn->child1, *(t_token*)tokens->content);
	fill_cmd_node(sn->child1, tokens);

	while (((t_token*)tokens->content)->type != PIPE)
		tokens = tokens->next;
	tokens = tokens->next;
	init_cmd_node(sn->child2, *(t_token*)tokens->content);
	fill_cmd_node(sn->child2, tokens);
	return sn;
}

t_script_node	*parse(t_dllist *tokens)
{
	if (!tokens)
		return create_and_add_error_node("no tokens");

	while (tokens->next) {
		if (((t_token*)tokens->content)->type == PIPE)
			return parse_pipe(tokens);
		tokens = tokens->next;
	}
	while (tokens->prev)
		tokens = tokens->prev;
	return (parse_cmd(tokens));
}
