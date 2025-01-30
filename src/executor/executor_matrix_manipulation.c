/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_matrix_manipulation.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:33:59 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 14:59:32 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	free_char_array(char **matrix)
{
	size_t	i;

	i = 0;
	if (!matrix)
		return ;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

char	**ep_to_matrix(int fds[2], t_list *ep)
{
	int		count;
	char	**env_matrix;
	char	*first;
	char	*second;

	env_matrix = (char **)malloc(sizeof(char *) * (ft_lstsize(ep) + 1));
	if (!env_matrix)
		teardown_close_fds(fds, "Allocation error");
	count = 0;
	while (ep)
	{
		first = ft_strjoin(((t_env_var *)ep->content)->key, "=");
		if (((t_env_var *)ep->content)->value)
			second = ft_strjoin(first, ((t_env_var *)ep->content)->value);
		else
			second = ft_strdup(first);
		env_matrix[count++] = ft_strdup(second);
		free(first);
		free(second);
		ep = ep->next;
	}
	env_matrix[count] = NULL;
	return (env_matrix);
}
