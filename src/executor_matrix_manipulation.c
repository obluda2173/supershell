/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_matrix_manipulation.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:33:59 by erian             #+#    #+#             */
/*   Updated: 2025/01/17 14:11:03 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

size_t ft_matrix_size(char **matrix)
{
    size_t size = 0;

    while (matrix && matrix[size])
        size++;
    return size;
}

char **ft_matrix_dup(char **matrix)
{
    if (!matrix)
        return NULL;

    size_t size = ft_matrix_size(matrix);
    char **new_matrix = malloc((size + 1) * sizeof(char *));
    if (!new_matrix)
        return NULL;

    for (size_t i = 0; i < size; i++)
    {
        new_matrix[i] = ft_strdup(matrix[i]);
        if (!new_matrix[i])
        {
            free_matrix(new_matrix);
            return NULL;
        }
    }
    new_matrix[size] = NULL;
    return new_matrix;
}

char **ft_matrix_join(char **m1, char **m2)
{
    size_t len1 = ft_matrix_size(m1);
    size_t len2 = ft_matrix_size(m2);

    // printf("%zu\n", len1);
    // printf("%zu\n", len2);
    
    char **result = malloc(sizeof(char *) * (len1 + len2 + 1));
    size_t i = 0;
    size_t j = -1;

    if (!result)
        return (NULL);
    while (i < len1)
    {
        result[i] = ft_strdup(m1[i]);
        i++;
    }
    while (++j < len2)
    {
        result[i] = ft_strdup(m2[j]);
        i++;
    }
    result[i] = NULL;
    return (result);
}
