/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_pwd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:43:42 by erian             #+#    #+#             */
/*   Updated: 2025/01/27 16:52:44 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	cstm_pwd(t_list **ep, t_cmd_node *cmd_node)
{
	t_list	*tmp_ep;
	t_env_var *tmp_var;

	(void)cmd_node;
	tmp_ep = *ep;
	while (tmp_ep)
	{
		tmp_var = (t_env_var *)tmp_ep->content;
		if (!ft_strcmp(tmp_var->key, "PWD"))
			printf("%s\n", tmp_var->value);
		tmp_ep = tmp_ep->next;
	}
	return (EXIT_SUCCESS);
}
