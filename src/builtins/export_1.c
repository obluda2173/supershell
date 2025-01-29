/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_export_1.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:37:32 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 16:47:40 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static bool	rewrite_var(t_list **ep, t_env_var *new_var)
{
	t_list		*tmp_ep;
	t_env_var	*tmp_var;

	tmp_ep = *ep;
	while (tmp_ep)
	{
		tmp_var = (t_env_var *)tmp_ep->content;
		if (ft_strcmp(tmp_var->key, new_var->key) == 0)
		{
			if (new_var->value)
			{
				free(tmp_var->value);
				tmp_var->value = ft_strdup(new_var->value);
			}
			free(new_var->value);
			free(new_var->key);
			free(new_var);
			return (true);
		}
		tmp_ep = tmp_ep->next;
	}
	return (false);
}

static bool	add_var(t_list **ep, t_list *arg_lst)
{
	t_list		*arg_i;
	t_env_var	*new_var;
	t_argument	*current_arg;

	arg_i = arg_lst;
	while (arg_i)
	{
		current_arg = (t_argument *)arg_i->content;
		new_var = malloc(sizeof(t_env_var));
		if (!new_var)
			return (false);
		if (!assign_var(&new_var, current_arg->word))
		{
			free(new_var);
			arg_i = arg_i->next;
			continue ;
		}
		if (!rewrite_var(ep, new_var))
			ft_lstadd_back(ep, ft_lstnew(new_var));
		arg_i = arg_i->next;
	}
	return (true);
}

static void	sort_for_export(t_list **ep)
{
	t_list		*tmp_lst;
	t_list		*prev;
	t_list		*last_node;
	int			list_size;

	if (!ep || !*ep)
		return ;
	sort_arguments(ep);
	tmp_lst = *ep;
	last_node = ft_lstlast(*ep);
	list_size = ft_lstsize(*ep);
	prev = NULL;
	while (list_size > 0 && tmp_lst)
	{
		move_invalid_keys(ep, &tmp_lst, &prev, &last_node);
		list_size--;
	}
}

static void	print_export(t_list **ep)
{
	t_list		*tmp_ep;
	t_list		**cpy_ep;
	t_env_var	*env_var;

	cpy_ep = copy_ep(*ep);
	sort_for_export(cpy_ep);
	tmp_ep = *cpy_ep;
	while (tmp_ep)
	{
		env_var = (t_env_var *)tmp_ep->content;
		if (env_var->key[0] == '_')
		{
			tmp_ep = tmp_ep->next;
			continue ;
		}
		if (env_var->value)
			print_env_var(env_var->key, env_var->value);
		else
			printf("declare -x %s\n", env_var->key);
		tmp_ep = tmp_ep->next;
	}
	ft_lstclear(cpy_ep, free_env_var);
	free(cpy_ep);
}

int	cstm_export(t_list **ep, t_cmd_node *cmd_node)
{
	if (!ft_lstsize(cmd_node->arguments))
	{
		print_export(ep);
		return (EXIT_SUCCESS);
	}
	if (!add_var(ep, cmd_node->arguments))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
