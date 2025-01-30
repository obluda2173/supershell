/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_pwd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:43:42 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 16:52:43 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <unistd.h>

int	cstm_pwd(t_list **ep, t_cmd_node *cmd_node)
{
	t_list		*tmp_ep;
	t_env_var	*tmp_var;

	(void)cmd_node;
	tmp_ep = *ep;
	while (tmp_ep)
	{
		tmp_var = (t_env_var *)tmp_ep->content;
		if (!ft_strcmp(tmp_var->key, "PWD"))
			ft_putendl_fd(tmp_var->value, STDOUT_FILENO);
		tmp_ep = tmp_ep->next;
	}
	return (EXIT_SUCCESS);
}
