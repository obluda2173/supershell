/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_exit.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:13:32 by erian             #+#    #+#             */
/*   Updated: 2025/01/28 15:34:00 by erian            ###   ########.fr       */
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

int cstm_exit(t_list *args)
{
    t_argument *arg;
    int exit_status = 0;

    if (ft_lstsize(args) > 1)
    {
        ft_putstr_fd("exit\nexit: too many arguments\n", STDERR_FILENO);
        return (EXIT_FAILURE);
    }
    if (ft_lstsize(args) > 0)
    {
        arg = (t_argument *)args->content;
        if (!valid_arg(arg->word))
        {
            ft_putstr_fd("exit\nminishell: numeric argument required\n", STDERR_FILENO);
            exit(2);
        }
        exit_status = ft_atoi(arg->word) % 256;
        if (exit_status < 0)
            exit_status += 256;
    }
    ft_putstr_fd("exit\n", STDOUT_FILENO);
    return (exit_status);
}
