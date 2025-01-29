/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_cd_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:20:33 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 14:13:54 by erian            ###   ########.fr       */
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

static void	update_dirs(t_list **ep)
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

static int	cd_home(t_list **ep)
{
	t_env_var	*home;

	home = get_env_var(*ep, "HOME");
	if (!home || !home->value)
	{
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (chdir(home->value) == -1)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		perror(home->value);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	cstm_cd(t_list **ep, t_list *args)
{
	if (check_args(args) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (!args)
	{
		if (cd_home(ep) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	else
	{
		if (chdir(((t_argument *)args->content)->word) == -1)
		{
			ft_putstr_fd("cd: ", STDERR_FILENO);
			perror(((t_argument *)args->content)->word);
			return (EXIT_FAILURE);
		}
	}
	update_dirs(ep);
	return (EXIT_SUCCESS);
}
