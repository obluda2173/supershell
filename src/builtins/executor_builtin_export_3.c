/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_export_3.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:04:24 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 16:50:48 by erian            ###   ########.fr       */
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

bool	should_be_escaped(char c)
{
	if (c == '$' || c == '\\' || c == '\"')
		return (true);
	return (false);
}

void	print_env_var(char *key, char *value)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(key, STDOUT_FILENO);
	ft_putstr_fd("=\"", STDOUT_FILENO);
	while (*value)
	{
		if (should_be_escaped(*value))
			ft_putchar_fd('\\', STDOUT_FILENO);
		ft_putchar_fd(*value++, STDOUT_FILENO);
	}
	ft_putendl_fd("\"", STDOUT_FILENO);
}

void	move_invalid_keys(t_list **ep, t_list **tmp_lst,
								t_list **prev, t_list **last_node)
{
	t_env_var	*env_var;

	env_var = (t_env_var *)(*tmp_lst)->content;
	if (!ft_isalpha(env_var->key[0]))
	{
		if (*prev)
			(*prev)->next = (*tmp_lst)->next;
		else
			*ep = (*tmp_lst)->next;
		(*last_node)->next = *tmp_lst;
		(*tmp_lst)->next = NULL;
		*last_node = *tmp_lst;
		if (*prev)
			*tmp_lst = (*prev)->next;
		else
			*tmp_lst = *ep;
	}
	else
	{
		*prev = *tmp_lst;
		*tmp_lst = (*tmp_lst)->next;
	}
}
