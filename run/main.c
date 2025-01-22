/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 13:59:08 by erian             #+#    #+#             */
/*   Updated: 2025/01/15 13:25:54 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include "executor.h"
#include <unistd.h>

# include <readline/readline.h>
# include <readline/history.h>

volatile sig_atomic_t signal_received = 0;

void handle_signals(int signum) {
	(void)signum;
	/* Replace the contents of rl_line_buffer with text. The point and mark are preserved, if possible.
	 * If clear_undo is non-zero, the undo list associated with the current line is cleared.  */
	rl_replace_line("", 0);

	ft_putendl_fd("", STDOUT_FILENO);
	/*  Tell the update routines that we have moved onto a new (empty) line, usually after ouputting a newline.  */
	rl_on_new_line();
	/*  Change what's displayed on the screen to reflect the current contents of rl_line_buffer.  */
	rl_redisplay();

	if (signum == SIGINT) {
		signal_received = 1;
	}
}

void get_input(t_data *data) {
	char* prompt;
	prompt = meeting_line(&data);
	data->line = rl_gets(prompt);
	free(prompt);
	prompt = NULL;
}

int repl(t_data *data) {
	while (!data->exit)
	{
		int pipefd[2];
		if (pipe(pipefd) == -1) {
			perror("pipe");
			return EXIT_FAILURE;
		}

		pid_t cpid = fork();
		if (cpid == -1) {
			perror("fork");
			return EXIT_FAILURE;
		}


		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		if (cpid == 0) {

			int fd_in = open("/dev/tty", O_RDONLY);
			if (fd_in != 0) {
				// Handle error or adjustment if needed
			}

			// Reopen STDOUT to the terminal
			int fd_out = open("/dev/tty", O_WRONLY);
			if (fd_out != 1) {
				// Handle error or adjustment if needed
			}
			close(pipefd[0]);
			get_input(data);
			if (data->line == NULL) {
				signal_received = 2;
				exit(EXIT_SUCCESS);
			}
			write(pipefd[1], data->line, ft_strlen(data->line));
			close(pipefd[1]);          /* Reader will see EOF */
			free_data(data);
			exit(EXIT_SUCCESS);
		}
		close(pipefd[1]);
		wait(NULL);
		int fd_in = open("/dev/tty", O_RDONLY);
		if (fd_in != 0) {
			// Handle error or adjustment if needed
		}

		// Reopen STDOUT to the terminal
		int fd_out = open("/dev/tty", O_WRONLY);
		if (fd_out != 1) {
			// Handle error or adjustment if needed
		}
		ft_putendl_fd("hello", STDOUT_FILENO);
		if (signal_received == 2) {
			data->exit = true;
			continue;
		}

		free(data->line);
		data->line = NULL;
		data->line = malloc(sizeof(char) * 100);
		*data->line = '\0';
		char* buf = data->line;
		while (read(pipefd[0], buf++, 1) > 0) {}
		*buf = '\0';
		close(pipefd[0]);

		if (signal_received == 1) {
			signal_received = 0;
			continue;
		}

		if (!data->line)
			break ;

		if (ft_strlen(data->line) == 0)
			continue ;

		if (ft_strncmp(data->line, "exit", 4) == 0)
		{
			data->exit = true;
			continue ;
		}

		if (!check_syntax(data->line))
		{
			data->exit_status = 2;
			continue ;
		}

		t_dllist *tokens = tokenize(data->line);
		if (!heredoc_loop(&tokens))
		{
			printf("Error: Heredoc processing failed.\n");
			ft_dllstclear(&tokens, free_token);
			continue ;
		}

		t_script_node *script = parse(tokens);
		ft_dllstclear(&tokens, free_token);

		if (script->node_type != ERROR_NODE)
			execute_script(script,  data);
		else {
			ft_putendl_fd((char*)script->node_data.error_node.error, STDERR_FILENO);
			data->exit_status = 2;
		}
		free_script_node(script);
	}
	return EXIT_SUCCESS;
}


int	main(int ac, char **av, char **ep)
{
	signal(SIGINT, handle_signals);
	t_data	*data;
	(void)av;

	if (ac != 1 || ep == NULL || *ep == NULL)
		return (printf("Error: No environment found. Exiting...\n"), 0);

	data = init(ep);

	if (!data)
		return (printf("Error: Initialization failed. Exiting...\n"), 0);

	if (repl(data) == EXIT_FAILURE ) {
		return EXIT_FAILURE;
		free_data(data);
	};
	free_data(data);
	return EXIT_SUCCESS;

}
