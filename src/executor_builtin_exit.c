/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_exit.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:13:32 by erian             #+#    #+#             */
/*   Updated: 2025/01/28 16:35:39 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static bool valid_arg(char *str)
{
    long long val = 0;
    int i = 0;
    int sign = 1;

    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return false;
        val = val * 10 + (str[i] - '0');
        if ((sign == 1 && val > LLONG_MAX) || (sign == -1 && -val < LLONG_MIN))
            return false;
        i++;
    }
    return true;
}

int cstm_exit(t_list *args, t_data *data)
{
    t_argument *arg;
    int exit_status = 0;
    
    ft_putstr_fd("exit\n", STDOUT_FILENO);

    if (args)
    {
        arg = (t_argument *)args->content;
        if (!valid_arg(arg->word))
        {
            ft_putstr_fd("minishell: numeric argument required\n", STDERR_FILENO);
            data->exit = true;
            return (2);
        }

        exit_status = ft_atoi(arg->word) % 256;
        if (exit_status < 0)
            exit_status += 256;

        if (ft_lstsize(args) > 1)
        {
            ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
            return (EXIT_FAILURE);
        }
    }

    data->exit = true;
    return (exit_status);
}
