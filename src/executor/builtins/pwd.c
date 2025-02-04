/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:43:42 by erian             #+#    #+#             */
/*   Updated: 2025/02/04 14:30:55 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	cstm_pwd(t_list **ep, t_cmd_node *cmd_node)
{
	char		*pwd;
	t_env_var	*tmp_var;

	(void)cmd_node;
	tmp_var = get_env_var(*ep, "PWD");
	if (tmp_var)
		ft_putendl_fd(tmp_var->value, STDOUT_FILENO);
	else
	{
		pwd = getcwd(NULL, 0);
		ft_putendl_fd(pwd, STDOUT_FILENO);
		free(pwd);
	}
	return (EXIT_SUCCESS);
}
