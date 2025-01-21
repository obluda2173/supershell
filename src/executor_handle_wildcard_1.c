/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard_1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:50:09 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 16:09:52 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char **finalize_result(char *result, char **argv)
{
	char	**split_result;
	char	**expanded_argv;

	if (!result || ft_strlen(result) == 0)
	{
		free(result);
		return (argv);
	}
	split_result = ft_split(result, ' ');
	expanded_argv = ft_matrix_join(argv, split_result);
	free_matrix(split_result);
	free(result);
	return (expanded_argv);
}

/*
r - result
rl - result length
dp - directory path
en - entry name
*/
void free_resources(DIR *dir, char *dir_path, char *pattern, char *result)
{
	if (dir)
		closedir(dir);
	if (dir_path)
		free(dir_path);
	if (pattern)
		free(pattern);
	if (result)
		free(result);
}
