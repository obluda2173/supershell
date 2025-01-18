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

void repl(t_data *data, char** ep) {
	char	*prompt;
	while (data->not_exit)
	{

		prompt = meeting_line(&data);
		data->line = rl_gets(prompt);
		free(prompt);
		prompt = NULL;

		if (!data->line)
			break ;

		//empty line handler
		if (ft_strlen(data->line) == 0)
			continue ;

		//exit the process
		if (ft_strncmp(data->line, "exit", 4) == 0)
		{
			data->not_exit = false;
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


		/* t_dllist *head = tokens; */
		/* while (head) { */
		/* 	printf("content: %s; type: %d\n", ((t_token*)head->content)->content, ((t_token*)head->content)->type); */
		/* 	head = head->next; */
		/* } */

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
	t_data	*data;
	(void)av;

	if (ac != 1 || ep == NULL || *ep == NULL)
		return (printf("Error: No environment found. Exiting...\n"), 0);

	data = init(ep);
	if (!data)
		return (printf("Error: Initialization failed. Exiting...\n"), 0);

	repl(data, ep);
	free_data(data);
	return (0);

}
