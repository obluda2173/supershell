/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_cd_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:56:37 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 14:14:05 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_env_var	*get_env_var(t_list *ep, char *key)
{
	t_list		*tmp_ep;
	t_env_var	*tmp_var;

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
int	check_args(t_list *args)
{
	if (ft_lstsize(args) > 1)
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
