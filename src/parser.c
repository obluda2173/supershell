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

t_list	*parse(t_dllist *tokens)
{
	t_list	*script;

	script = NULL;

	if (!tokens) {
		create_and_add_error_node(&script, "no tokens");
		return script;
	}

	while (tokens->next) {
		if (((t_token*)tokens->content)->type == PIPE) {
			t_script_node* sn = (t_script_node*)malloc(sizeof(t_script_node));
			sn->node_type = PIPE_NODE;
			sn->num_children=2;
			sn->child1 = malloc(sizeof(t_script_node));
			if (!sn->child1) {
				free(sn);
				create_and_add_error_node(&script, "no tokens");
				return script;
			}

			init_cmd_node(sn->child1);
			return ft_lstnew(sn);
		}
		tokens = tokens->next;
	}
	while (tokens->prev)
		tokens = tokens->prev;
	return (parse_cmd(script, tokens));
}
