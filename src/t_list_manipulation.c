/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_list_manipulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:16:08 by erian             #+#    #+#             */
/*   Updated: 2024/12/28 15:29:44 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_list	*t_lst_last(t_token_list *token_list)
{
	if (!token_list)
		return (NULL);
	while (token_list->next)
		token_list = token_list->next;
	return (token_list);
}

void	t_lst_append(t_token_list *token_list, t_token token)
{
	t_token_list *new_node;
	t_token_list *last_node;
    
	last_node = t_lst_last(token_list);
	if (!last_node)
		*token_list = token;
	else
		last_node->next-> = token;
	
}
