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

char	*get_path_env(t_list *envp)
{
	while (envp)
	{
		if (ft_strncmp((char *)envp->content, "PATH=", 5) == 0)
			return ((char *)envp->content + 5);
		envp = envp->next;
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

char	*find_path(char *cmd, char *path_env)
{
	char	**paths;
	char	*final_path;

	if (!cmd || !path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	final_path = search_in_paths(cmd, paths);
	free_matrix(paths);
	return (final_path);
}
