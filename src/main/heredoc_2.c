/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:33:59 by erian             #+#    #+#             */
/*   Updated: 2025/01/31 14:34:50 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_heredoc_process(int pipefd[2], pid_t *cpid)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (0);
	}
	*cpid = fork();
	if (*cpid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
	}
	if (*cpid == 0)
	{
		child_heredoc(pipefd);
		exit(0);
	}
	close(pipefd[1]);
	wait(NULL);
	return (1);
}

static int	handle_heredoc_signal(t_data *data, char **heredoc_input)
{
	if (g_signal_received != 1)
		return (0);
	data->exit_status = 130;
	g_signal_received = 0;
	free(*heredoc_input);
	*heredoc_input = NULL;
	return (1);
}

static char	*append_line(char *heredoc_input, char *line)
{
	char	*tmp;

	tmp = ft_strjoin(heredoc_input, line);
	free(heredoc_input);
	heredoc_input = ft_strjoin(tmp, "\n");
	free(tmp);
	free(line);
	if (!heredoc_input)
		printf("Error: Memory allocation failed 1.\n");
	return (heredoc_input);
}

static int	is_delimiter_reached(char *line, char *delimiter)
{
	return (line == NULL || ft_strcmp(line, delimiter) == 0);
}

char	*read_heredoc_input(char *delimiter, t_data *data)
{
	char	*heredoc_input;
	char	*line;
	int		pipefd[2];
	pid_t	cpid;

	heredoc_input = ft_strdup("");
	while (1)
	{
		if (!create_heredoc_process(pipefd, &cpid))
		{
			free(heredoc_input);
			return (NULL);
		}
		if (handle_heredoc_signal(data, &heredoc_input))
			return (NULL);
		line = read_line_from_fd(pipefd[0]);
		if (is_delimiter_reached(line, delimiter))
		{
			free(line);
			break ;
		}
		heredoc_input = append_line(heredoc_input, line);
		if (!heredoc_input)
			return (NULL);
	}
	return (heredoc_input);
}
