/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:04:24 by erian             #+#    #+#             */
/*   Updated: 2025/01/27 15:58:50 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static t_env_var	*copy_var(t_env_var *var)
{
	t_env_var	*ret_var;
	char		*key;
	char		*value;

	ret_var = malloc(sizeof(t_env_var));
	if (!ret_var)
		return (NULL);
	key = ft_strdup(var->key);
	if (var->value)
		value = ft_strdup(var->value);
	else
		value = NULL;
	if (!key)
	{
		free(key);
		if (value)
			free(value);
		free(ret_var);
		return (NULL);
	}
	ret_var->key = key;
	ret_var->value = value;
	return (ret_var);
}

t_list	**copy_ep(t_list *ep)
{
	t_list		*tmp_ep;
	t_list		**ret_ep;
	t_env_var	*tmp_var;
	t_env_var	*ret_var;

	ret_ep = malloc(sizeof(t_list *));
	if (!ret_ep)
		return (NULL);
	*ret_ep = NULL;
	
	tmp_ep = ep;

	while (tmp_ep)
	{
		tmp_var = (t_env_var *)tmp_ep->content;
		ret_var = copy_var(tmp_var);
		if (!ret_var)
		{
			ft_lstclear(ret_ep, free_env_var);
			free(ret_ep);
			return (NULL);
		}
		ft_lstadd_back(ret_ep, ft_lstnew(ret_var));

		tmp_ep = tmp_ep->next;
	}
	return (ret_ep);
}
