/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_modify_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:03/48 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 15:03:48 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* t1->t2->other_tokens->t3->...->EOF becomes t1->t2->other_tokens->EOF */
t_script_node	*truncate_tokens(t_script_node *sn, t_dllist **tokens)
{
	*tokens = (*tokens)->prev;
	if (!*tokens)
		return (teardown(sn, "error parsing pipeline before logical operator"));
	ft_dllstclear(&((*tokens)->next), free_token);
	ft_dllstadd_back(tokens, ft_dllstnew(create_token(NULL, END_OF_FILE)));
	return (NULL);
}

t_script_node	*set_downstream_tokens(t_dllist **downstream_tokens,
		t_dllist *tokens, t_script_node *sn)
{
	t_script_node	*err;

	if (((t_token *)tokens->next->content)->type == LPAREN)
	{
		tokens->next->next->prev = NULL;
		*downstream_tokens = tokens->next->next;
		err = remove_next_rparen(*downstream_tokens, sn);
		if (err)
			return (err);
	}
	else
	{
		tokens->next->prev = NULL;
		*downstream_tokens = tokens->next;
	}
	return (NULL);
}
