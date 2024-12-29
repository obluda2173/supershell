/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 12:39:03 by erian             #+#    #+#             */
/*   Updated: 2024/12/29 14:10:32 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_token(void *content)
{
	t_token *token;

	token = (t_token *)content;
	free(token->content);
	free(token);
}

void free_ep(t_list **ep)
{
	t_list	*current_ep;
	t_list	*temp_ep;
	
	current_ep = *ep;
	while (current_ep)
	{
		
		temp_ep = current_ep->next;
		free(current_ep);
		current_ep = temp_ep;
	}
}

void	free_all(t_data **data)
{
	if (!data || !*data)
		return;

	//free token doubly list
	if ((*data)->tokens)
		ft_dllstclear(&(*data)->tokens, free_token);

	//free environment ponter list
	if ((*data)->ep)
		free_ep(&(*data)->ep);
	
	//free line of commands
	if ((*data)->line)
		free((*data)->line);
		
	free(*data);
	*data = NULL;
}

