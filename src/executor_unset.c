/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 16:17:14 by erian             #+#    #+#             */
/*   Updated: 2025/01/27 12:14:26 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void free_var(t_list *ep)
{
    t_env_var *var;

    var = (t_env_var *)ep->content;
    free(var->key);
    free(var->value);
    free(var);
}

int cstm_unset(t_list **ep, t_cmd_node *cmd_node)
{
    t_list *tmp_ep;
    t_list *prev_ep;
    t_list *arg_lst;
    t_argument *arg;
    t_env_var *env_var;

    arg_lst = (t_list *)cmd_node->arguments;
    if (!ft_lstsize(cmd_node->arguments))
        return (EXIT_SUCCESS);
    while (arg_lst)
    {
        arg = (t_argument *)arg_lst->content;
        tmp_ep = *ep;
        prev_ep = NULL;
        while (tmp_ep)
        {
            env_var = (t_env_var *)tmp_ep->content;
            if (ft_strcmp(arg->word, env_var->key) == 0)
            {
                if (prev_ep)
                    prev_ep->next = tmp_ep->next;
                else
                    *ep = tmp_ep->next;
                free_var(tmp_ep);
                free(tmp_ep);
                break;
            }
            prev_ep = tmp_ep;
            tmp_ep = tmp_ep->next;
        }
        arg_lst = arg_lst->next;
    }
    return (EXIT_SUCCESS);
}
