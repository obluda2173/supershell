/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:31:12 by erian             #+#    #+#             */
/*   Updated: 2025/01/09 13:31:36 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void free_matrix(char **matrix)
{
    size_t i = 0;

    if (!matrix)
        return;

    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
}
