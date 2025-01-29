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

size_t	ft_size_char_array(char **matrix)
{
	size_t	size;

	size = 0;
	while (matrix && matrix[size])
		size++;
	return (size);
}

char	**ft_matrix_join(char **m1, char **m2)
{
	char	**result;
	size_t	i;
	size_t	j;

	i = -1;
	j = -1;
	result = malloc(sizeof(char *) * (ft_size_char_array(m1)
				+ ft_size_char_array(m2) + 1));
	if (!result)
		return (NULL);
	while (++i < ft_size_char_array(m1))
		result[i] = ft_strdup(m1[i]);
	while (++j < ft_size_char_array(m2))
		result[i++] = ft_strdup(m2[j]);
	result[i] = NULL;
	return (result);
}
