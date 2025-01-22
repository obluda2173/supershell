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


volatile sig_atomic_t sigint_received = 0;

void handle_signals(int signum) {
	(void)signum;
	sigint_received = 1;
	/* Replace the contents of rl_line_buffer with text. The point and mark are preserved, if possible.
	 * If clear_undo is non-zero, the undo list associated with the current line is cleared.  */
	rl_replace_line("", 0);

	ft_putendl_fd("", STDOUT_FILENO);
	/*  Tell the update routines that we have moved onto a new (empty) line, usually after ouputting a newline.  */
	rl_on_new_line();
	/*  Change what's displayed on the screen to reflect the current contents of rl_line_buffer.  */
	rl_redisplay();

}

void get_input(t_data *data) {
	char* prompt;
	prompt = meeting_line(&data);
	data->line = rl_gets(prompt);
	free(prompt);
	prompt = NULL;
}

void repl(t_data *data) {
	while (!data->exit)
	{
        if (sigint_received) {
            sigint_received = 0;
        }
		get_input(data);
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
		data->line = NULL;
	}
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

	repl(data);
	free_data(data);
	return (0);

}
