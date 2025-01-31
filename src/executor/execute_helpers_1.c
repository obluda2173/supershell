/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helpers_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 19:12/53 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/31 19:12:53 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		perror("stat");
		return (0);
	}
	return (S_ISDIR(path_stat.st_mode));
}

int	error_cmd_not_found(t_cmd_node *cn)
{
	ft_putstr_fd("Command not found: ", STDERR_FILENO);
	ft_putendl_fd(cn->cmd_token.content, STDERR_FILENO);
	return (127);
}

int	error_is_directory(char *cmd_path)
{
	ft_putendl_fd("Is a directory", STDERR_FILENO);
	free(cmd_path);
	return (126);
}
