/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_cd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:20:33 by erian             #+#    #+#             */
/*   Updated: 2025/01/28 11:29:25 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_env_var	*get_env_var(t_list *ep, char *key)
{
	t_list *tmp_ep;
	t_env_var *tmp_var;

	tmp_ep = ep;
	while (tmp_ep)
	{
		tmp_var = (t_env_var *)tmp_ep->content;
		if (!ft_strcmp(tmp_var->key, key))
			return (tmp_var);
		tmp_ep = tmp_ep->next;
	}
	return (NULL);
}

static void update_dirs(t_list **ep)
{
    char *tmp = getcwd(NULL, 0);
    t_env_var *oldpwd = get_env_var(*ep, "OLDPWD");
    t_env_var *pwd = get_env_var(*ep, "PWD");

    if (!tmp)
    {
        perror("getcwd");
        return ;
	}
    if (oldpwd)
    {
		free(oldpwd->value);
		if (pwd && pwd->value)
			oldpwd->value = ft_strdup(pwd->value);
		else
			oldpwd->value = ft_strdup(tmp);
    }
    if (pwd)
    {
        free(pwd->value);
        pwd->value = tmp;
    }
    else
        free(tmp); 
}

static int check_args(t_list *args)
{
    if (ft_lstsize(args) > 1)
    {
        ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int cstm_cd(t_list **ep, t_list *args)
{
    t_env_var *home;

    if (check_args(args) != EXIT_SUCCESS)
        return (EXIT_FAILURE);
    if (!args)
    {
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
