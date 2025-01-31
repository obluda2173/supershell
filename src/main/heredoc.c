/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:00:40 by erian             #+#    #+#             */
/*   Updated: 2025/01/31 17:26:02 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signals_heredoc(int signum)
{
	(void)signum;
	ft_putendl_fd("", STDOUT_FILENO);
	exit(EXIT_FAILURE);
}

void	child_heredoc(int pipefd[2])
{
	char	*line;

	signal(SIGINT, handle_signals_heredoc);
	close(pipefd[0]);
	line = readline("heredoc> ");
	if (line == NULL)
	{
		ft_putendl_fd("warning: heredoc delimited by end-of-file",
			STDOUT_FILENO);
		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	}
	write(pipefd[1], line, ft_strlen(line) + 1);
	close(pipefd[1]);
	free(line);
	exit(EXIT_SUCCESS);
}

int	count_non_quotes(char *str)
{
	int	count;

	count = 0;
	if (!str)
		return (count);
	while (*str)
	{
		if (!(*str == '\'' || *str == '\"'))
			count++;
		str++;
	}
	return (count);
}

char	*remove_quotes(char *str)
{
	int		final_len;
	char	*new_str;
	int		i;

	if (!str)
		return (str);
	if (*str == '\0')
		return (str);
	final_len = count_non_quotes(str);
	new_str = malloc(sizeof(char) * (final_len + 1));
	if (!new_str)
	{
		ft_putendl_fd("Allocation error", STDERR_FILENO);
		return (NULL);
	}
	new_str[final_len] = '\0';
	i = 0;
	while (*str)
	{
		if (!(*str == '\'' || *str == '\"'))
			new_str[i++] = *str;
		str++;
	}
	return (new_str);
}

int	heredoc_loop(t_dllist **tokens, t_data *data)
{
	t_dllist	*heredoc_token;
	char		*delimiter;
	bool		quoted;
	char		*heredoc_input;
	t_dllist	*new_node;

	heredoc_token = search_heredoc(*tokens);
	if (!heredoc_token)
		return (EXIT_SUCCESS);
	if (process_heredoc_delimiter(&heredoc_token, data, &delimiter, &quoted))
		return (EXIT_FAILURE);
	heredoc_input = read_heredoc_input(delimiter, data);
	free(delimiter);
	if (!heredoc_input)
		return (EXIT_FAILURE);
	new_node = create_heredoc_token(heredoc_input, quoted);
	if (!new_node)
		return (EXIT_FAILURE);
	return (replace_token(heredoc_token, new_node, data));
}
