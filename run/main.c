/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:59:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/26 09:30:33 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void	handle_signals_2(int signum)
{
	(void)signum;
	signal_received = 1;
}

void	handle_signals(int signum)
{
	(void)signum;
	/* Replace the contents of rl_line_buffer with text. The point and mark are preserved,
	   if possible.
	   * If clear_undo is non-zero,
	   the undo list associated with the current line is cleared.  */
	rl_replace_line("", 0);
	ft_putendl_fd("", STDOUT_FILENO);
	/*  Tell the update routines that we have moved onto a new (empty) line,
		usually after ouputting a newline.  */
	rl_on_new_line();
	/*  Change what's displayed on the screen to reflect the current contents of rl_line_buffer.  */
	rl_redisplay();
	if (signum == SIGINT)
	{
		signal_received = 1;
	}
}

void	get_input(t_data *data)
{
	char	*prompt;

	prompt = meeting_line(&data);
	data->line = rl_gets(prompt);
	free(prompt);
	prompt = NULL;
}

void	child_input(int pipefd[2], t_data *data)
{
	signal(SIGINT, handle_signals);
	close(pipefd[0]);
	get_input(data);
	if (data->line == NULL)
	{
		close(pipefd[1]); /* Reader will see EOF */
		free_data(data);
		exit(EXIT_SUCCESS);
	}
	write(pipefd[1], data->line, ft_strlen(data->line) + 1);
	close(pipefd[1]); /* Reader will see EOF */
	free_data(data);
	exit(EXIT_SUCCESS);
}

void	parse_and_execute(t_dllist *tokens, t_data *data)
{
	t_script_node	*script;

	script = parse(tokens);
	ft_dllstclear(&tokens, free_token);
	if (script->node_type != ERROR_NODE)
		execute_script(script, data);
	else
	{
		ft_putendl_fd((char *)script->node_data.error_node.error,
			STDERR_FILENO);
		data->exit_status = 2;
	}
	free_script_node(script);
}

bool	check_data(t_data *data)
{
	if (!data->line || !ft_strncmp(data->line, "exit", 4))
	{
		data->exit = true;
		return (false);
	}
	if (ft_strlen(data->line) == 0)
	{
		free(data->line);
		data->line = NULL;
		return (false);
	}
	if (!check_syntax(data->line))
	{
		data->exit_status = 2;
		return (false);
	}
	if (signal_received == 1)
	{
		data->exit_status = 130;
		signal_received = 0;
	}
	return (true);
}

char	*minishell_input(t_data *data)
{
	int		pipefd[2];
	pid_t	cpid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (NULL);
	}
	cpid = fork();
	if (cpid == -1)
	{
		perror("fork");
		return (NULL);
	}
	if (cpid == 0)
		child_input(pipefd, data);
	close(pipefd[1]);
	wait(NULL);
	free(data->line);
	data->line = NULL;
	return (read_line_from_child(pipefd[0]));
}

int	repl(t_data *data)
{
	t_dllist	*tokens;

	signal(SIGINT, handle_signals_2); // Parent ignores SIGINT
	while (!data->exit)
	{
		data->line = minishell_input(data);
		if (!check_data(data)) {
			free(data->line);
			data->line = NULL;
			continue ;
		}
		add_history(data->line);
		tokens = tokenize(data->line);
		free(data->line);
		data->line = NULL;
		if (heredoc_loop(&tokens, data))
		{
			/* printf("Error: Heredoc processing failed.\n"); */
			ft_dllstclear(&tokens, free_token);
			continue ;
		}
		parse_and_execute(tokens, data);
	}
	return (EXIT_SUCCESS);
}

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
	};
	free_data(data);
	return (EXIT_SUCCESS);
}
