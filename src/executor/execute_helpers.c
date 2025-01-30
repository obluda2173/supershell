/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:18/27 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 18:18:27 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	close_fds(int fds[2])
{
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	return ;
}

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

int	teardown_close_fds(int fds[2], char *err_msg)
{
	perror(err_msg);
	close_fds(fds);
	return (EXIT_FAILURE);
}
