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

int	echo(t_cmd_node cmd_node, int fds[2])
{
	char	**argv;
	int		res;
	char	**head;
	bool	print_newline;

	argv = NULL;
	argv = list_to_argv(cmd_node.arguments, "");
	res = 0;
	if (!argv)
		return (1);
	head = argv;
	head++;
	if (*head)
	{
		print_newline = true;
		if (!ft_strcmp(*head, "-n"))
		{
			print_newline = false;
			head++;
		}
		while (*head)
		{
			ft_putstr_fd(*head, fds[1]);
			head++;
			if (*head)
				ft_putstr_fd(" ", fds[1]);
		}
		if (print_newline)
			ft_putendl_fd("", fds[1]);
	}
	else
	{
		ft_putendl_fd("", fds[1]);
	}
	free_char_array(argv);
	return (res);
}
