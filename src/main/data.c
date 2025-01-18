/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:23/59 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/18 18:23:59 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <unistd.h>

// initialise data structure and extract environment
t_data	*init(char **ep)
{
	t_list	*new_node;
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->ep = NULL;
	data->exit_status = 0;
	data->not_exit = true;
	data->line = NULL;
	while (*ep != NULL)
	{
		new_node = ft_lstnew(ft_strdup(*ep));
		if (!new_node)
		{
			free_data(data);
			return (NULL);
		}
		ft_lstadd_back(&(data->ep), new_node);
		ep++;
	}
	return (data);
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->ep)
		ft_lstclear(&(data->ep), free);
	if (data->line)
		free(data->line);
	free(data);
}
