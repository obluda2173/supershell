/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:07/43 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 09:07:43 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rl_gets(const char *prompt)
{
	char	*line_read;

	line_read = readline(prompt);
	if (line_read && *line_read)
		add_history(line_read);
	return (line_read);
}

char	*get_logname(t_env_var *env_var)
{
	char	*line;
	char	*result;

	result = NULL;
	if (ft_strcmp(env_var->key, "LOGNAME") == 0)
	{
		if (env_var->value)
			line = ft_strdup(env_var->value);
		else
			line = ft_strdup("");
		if (line)
		{
			result = ft_strjoin(line, "$ ");
			free(line);
		}
	}
	return (result);
}

char	*get_prompt(t_data **data)
{
	t_list		*start;
	char		*result;
	t_env_var	*env_var;

	/* char		*line; */
	if (!data || !*data)
		return (NULL);
	start = (*data)->ep;
	result = NULL;
	while (start)
	{
		env_var = (t_env_var *)start->content;
		result = get_logname(env_var);
		if (result)
			return (result);
		start = start->next;
	}
	return (ft_strdup("$ "));
}

void	child_input(int pipefd[2], t_data *data)
{
	char	*prompt;

	signal(SIGINT, handle_signals);
	close(pipefd[0]);
	prompt = get_prompt(&data);
	data->line = rl_gets(prompt);
	free(prompt);
	prompt = NULL;
	if (data->line == NULL)
	{
		close(pipefd[1]);
		free_data(data);
		exit(EXIT_SUCCESS);
	}
	write(pipefd[1], data->line, ft_strlen(data->line) + 1);
	close(pipefd[1]);
	free_data(data);
	exit(EXIT_SUCCESS);
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
	return (read_line_from_fd(pipefd[0]));
}
