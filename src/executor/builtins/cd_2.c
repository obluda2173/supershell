/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_cd_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:56:37 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 15:01:44 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	update_oldpwd(t_list **ep, char *tmp)
{
	t_env_var	*oldpwd;
	t_env_var	*pwd;

	oldpwd = get_env_var(*ep, "OLDPWD");
	pwd = get_env_var(*ep, "PWD");
	if (oldpwd)
	{
		free(oldpwd->value);
		if (pwd && pwd->value)
			oldpwd->value = ft_strdup(pwd->value);
		else
			oldpwd->value = ft_strdup(tmp);
	}
}

void	update_dirs(t_list **ep)
{
	char		*tmp;
	t_env_var	*pwd;

	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		perror("getcwd");
		return ;
	}
	update_oldpwd(ep, tmp);
	pwd = get_env_var(*ep, "PWD");
	if (pwd)
	{
		free(pwd->value);
		pwd->value = tmp;
	}
	else
		free(tmp);
}

int	check_args(t_list *args)
{
	if (ft_lstsize(args) > 1)
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
