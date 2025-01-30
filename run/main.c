/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:59:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/28 15:36:02 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **ep)
{
	t_data	*data;

	(void)av;
	if (ac != 1 || ep == NULL || *ep == NULL)
		return (printf("Error: No environment found. Exiting...\n"), 0);
	data = init(ep);
	if (!data)
		return (printf("Error: Initialization failed. Exiting...\n"), 0);
	if (repl(data) == EXIT_FAILURE)
	{
		return (EXIT_FAILURE);
		free_data(data);
	}
	free_data(data);
	return (EXIT_SUCCESS);
}
