/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:13:32 by erian             #+#    #+#             */
/*   Updated: 2025/02/04 11:16:38 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static bool	valid_arg(char *s)
{
	long long unsigned int	val;
	int						i;

	val = 0;
	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		val = val * 10 + (s[i] - '0');
		if (val > LONG_MAX)
			return (false);
		i++;
	}
	return (true);
}

static void	i_hate_norminette(void)
{
	ft_putstr_fd("minishell: numeric argument required\n", STDERR_FILENO);
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
			i_hate_norminette();
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
