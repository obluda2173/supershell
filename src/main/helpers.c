/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:52/04 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 09:52:04 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	error_fork(void)
{
	perror("fork");
	return (EXIT_FAILURE);
}

char	*error_and_return_null(char *msg)
{
	ft_putendl_fd(msg, STDERR_FILENO);
	return (NULL);
}
