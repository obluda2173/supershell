/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:28:28 by erian             #+#    #+#             */
/*   Updated: 2025/01/27 13:00:09 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	cstm_env(t_list **ep, t_cmd_node *cmd_node)
{
	t_list	*tmp_ep;
	t_env_var *tmp_var;

	if (ft_lstsize(cmd_node->arguments))
		return (EXIT_FAILURE);
	tmp_ep = *ep;
	while (tmp_ep)
	{
		tmp_var = (t_env_var *)tmp_ep->content;
		if (tmp_var->value)
			printf("%s=%s\n", tmp_var->key, tmp_var->value);
		tmp_ep = tmp_ep->next;
	}
	return (EXIT_SUCCESS);
}
