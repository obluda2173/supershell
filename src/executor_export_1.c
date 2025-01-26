/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:37:32 by erian             #+#    #+#             */
/*   Updated: 2025/01/26 13:49:02 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static bool add_expansions(t_list **ep, t_list *arg_lst)
{
    t_list      *arg_i;
    t_env_var   *new_var;
    t_argument  *current_arg;

    arg_i = arg_lst;
    while (arg_i)
    {
        current_arg = (t_argument *)arg_i->content;
        
        new_var = malloc(sizeof(t_env_var));
        
        if (!assign_var(&new_var, current_arg->word))
        {
            free(new_var);
            return (false);
        }
        ft_lstadd_back(ep, ft_lstnew(new_var));
        arg_i = arg_i->next;
    }
    return (true);
}

static void sort_for_export(t_list **ep)
{
    t_list *tmp_lst, *prev, *last_node;
    t_env_var *env_var;
    int list_size;

    if (!ep || !*ep)
        return;

    sort_arguments(ep);
    tmp_lst = *ep;
    last_node = ft_lstlast(*ep);
    list_size = ft_lstsize(*ep);
    prev = NULL;

    while (list_size > 0 && tmp_lst)
    {
        env_var = (t_env_var *)tmp_lst->content;
        if (!ft_isalpha(env_var->key[0]))
        {
            if (prev)
                prev->next = tmp_lst->next;
            else
                *ep = tmp_lst->next;

            last_node->next = tmp_lst;
            tmp_lst->next = NULL;
            last_node = tmp_lst;

            tmp_lst = (prev) ? prev->next : *ep;
        }
        else
        {
            prev = tmp_lst;
            tmp_lst = tmp_lst->next;
        }
        list_size--;
    }
}

static void	print_export(t_list **ep)
{
	t_list	*tmp_ep;
	t_env_var	*env_var;
	
	sort_for_export(ep);
    tmp_ep = *ep;
	while (tmp_ep)
	{
		env_var = (t_env_var *)tmp_ep->content;

        if (env_var->value)
            printf("declare -x %s=\"%s\"\n", env_var->key, env_var->value);
        else
            printf("declare -x %s\n", env_var->key);
        tmp_ep = tmp_ep->next;
	}
}

int	export(t_list **ep, t_cmd_node *cmd_node)
{
	if (!ft_lstsize(cmd_node->arguments))
	{
		print_export(ep);
		return EXIT_SUCCESS;
	}
	
	if (!add_expansions(ep, cmd_node->arguments))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
