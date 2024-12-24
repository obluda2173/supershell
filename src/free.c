/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 12:39:03 by erian             #+#    #+#             */
/*   Updated: 2024/12/24 13:41:26 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void free_all(t_data **data)
{
	if (!data || !*data)
		return;

	t_ep *current = (*data)->ep;
	t_ep *temp;

	while (current)
	{
		
		temp = current->next;
		free(current->value);
		free(current);
		current = temp;
	}

	if ((*data)->line)
		free((*data)->line);

	free(*data);
	*data = NULL;
}
