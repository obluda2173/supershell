/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_exit.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:13:32 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 15:06:51 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static bool	valid_arg(char *str)
{
	long long	val;
	int			i;
	int			s;

	val = 0;
	i = 0;
	s = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			s = -1;
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		val = val * 10 + (str[i] - '0');
		if ((s == 1 && val > LLONG_MAX) || (s == -1 && (-val < LLONG_MIN)))
			return (false);
		i++;
	}
	return (true);
}

int	cstm_exit(t_list *args, t_data *data)
{
	t_argument	*arg;
	int			exit_status;

	exit_status = 0;
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
