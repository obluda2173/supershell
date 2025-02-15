/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 07:29/29 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 07:29:29 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

char	*get_path_env(t_list *ep)
{
	t_env_var	*env_var;

	while (ep)
	{
		env_var = (t_env_var *)ep->content;
		if (ft_strcmp(env_var->key, "PATH") == 0)
			return (env_var->value);
		ep = ep->next;
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
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	final_path = search_in_paths(cmd, paths);
	free_char_array(paths);
	return (final_path);
}
