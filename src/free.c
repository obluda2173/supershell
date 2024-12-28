/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 12:39:03 by erian             #+#    #+#             */
/*   Updated: 2024/12/25 16:58:21 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token_list *tokens)
{
	t_token_list	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->content)
			free(tokens->content);
		free(tokens);
		tokens = temp;
	}
}

void free_all(t_data **data)
{
	t_ep 	*current_ep;
	t_ep 	*temp_ep;
	
	if (!data || !*data)
		return;
	if ((*data)->tokens)
		free_tokens((*data)->tokens);
	if ((*data)->ep)
	{
		current_ep = (*data)->ep;
		while (current_ep)
		{
			
			temp_ep = current_ep->next;
			free(current_ep->value);
			free(current_ep);
			current_ep = temp_ep;
		}
	}
	if ((*data)->line)
		free((*data)->line);
	free(*data);
	*data = NULL;
}
