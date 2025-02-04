/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 08:59:28 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/03 18:36:13 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <unistd.h>

void	parse_and_execute(t_dllist *tokens, t_data *data)
{
	t_script_node	*script;

	script = parse(tokens);
	ft_dllstclear(&tokens, free_token);
	if (!script)
	{
		data->exit_status = 0;
		return ;
	}
	if (script->node_type != ERROR_NODE)
		data->exit_status = execute_script(script, data);
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
	if (!data->line)
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
	add_history(data->line);
	if (!check_syntax(data->line))
	{
		data->exit_status = 2;
		return (false);
	}
	if (g_signal_received == 1)
	{
		data->exit_status = 130;
		g_signal_received = 0;
	}
	return (true);
}

int	repl(t_data *data)
{
	t_dllist	*tokens;

	signal(SIGINT, handle_signals_2);
	signal(SIGQUIT, SIG_IGN);
	while (!data->exit)
	{
		data->line = minishell_input(data);
		if (!check_data(data))
		{
			free(data->line);
			data->line = NULL;
			continue ;
		}
		tokens = tokenize(data->line);
		free(data->line);
		data->line = NULL;
		if (heredoc_loop(&tokens, data))
		{
			ft_dllstclear(&tokens, free_token);
			continue ;
		}
		parse_and_execute(tokens, data);
	}
	return (EXIT_SUCCESS);
}
