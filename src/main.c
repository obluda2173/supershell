/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:59:08 by erian             #+#    #+#             */
/*   Updated: 2024/12/23 18:12:50 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

void	process_line(char *shell)
{
	char *fullpath;
	
	shell->command->
}

void    minishell_loop(char **shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell% ");
		if (!line)
		{
			printf("EOF received. Exiting...\n");
			break;
		}
		shell->line = *line;
		process_line(shell);
		free(line);
	}
}

void	init_shell(t_shell *shell, char **ep)
{
	shell->line = NULL;
	shell->ep = *ep;
}

int main(int ac, char **av, char **ep)
{
	t_shell	*shell;

	//initial mandatory check
	if (ac != 1 || ep == NULL || *ep == NULL)
		return (printf("Error: No environment found. Exiting...\n"), 0);
		
	//suppress warning of unused argument
	(void)av;

	shell = malloc(sizeof(t_shell));

	init_shell(&shell, ep);
	
	//main loop
	minishell_loop(shell);
	
	return (0);
}
