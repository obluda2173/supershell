/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:17:23 by erian             #+#    #+#             */
/*   Updated: 2025/01/09 17:10:09 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include <unistd.h>

static char	*get_paths(char **ep)
{
	int	i;

	i = 0;
	if (!ep)
		return (NULL);
	while (ep[i] != NULL)
	{
		if (ft_strncmp(ep[i], "PATH=", 5) == 0)
			return (ep[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*search_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*part_path;
	char	*final_path;

	i = 0;
	while (paths[i] != NULL)
	{
		part_path = ft_strjoin(paths[i], "/");
		if (!part_path)
		{
			i++;
			continue ;
		}
		final_path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (final_path && access(final_path, F_OK) == 0)
			return (final_path);
		free(final_path);
		i++;
	}
	return (NULL);
}

char	*find_path(char *cmd, char **ep)
{
	char	*path_env;
	char	**paths;
	char	*final_path;

	path_env = get_paths(ep);
	if (!cmd || !path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	final_path = search_in_paths(cmd, paths);
	free_matrix(paths);
	return (final_path);
}
