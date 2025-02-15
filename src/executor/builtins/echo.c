/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 06:58/19 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 06:58:19 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	only_ns(char *str)
{
	if (!str || !*str)
		return (false);
	if (*str != '-')
		return (false);
	str++;
	while (*str)
	{
		if (*str != 'n')
			return (false);
		str++;
	}
	return (true);
}

void	echo_print(int fds[2], char **head)
{
	bool	print_newline;

	print_newline = true;
	if (only_ns(*head))
	{
		print_newline = false;
		head++;
	}
	while (*head)
	{
		if (only_ns(*head))
		{
			head++;
			continue ;
		}
		ft_putstr_fd(*head, fds[1]);
		head++;
		if (*head)
			ft_putstr_fd(" ", fds[1]);
	}
	if (print_newline)
		ft_putendl_fd("", fds[1]);
}

int	echo(t_cmd_node cmd_node, int fds[2])
{
	char	**argv;
	int		res;
	char	**head;

	argv = NULL;
	argv = list_to_argv(cmd_node.arguments, "");
	res = 0;
	if (!argv)
		return (1);
	head = argv;
	head++;
	if (*head)
		echo_print(fds, head);
	else
		ft_putendl_fd("", fds[1]);
	free_char_array(argv);
	return (res);
}
