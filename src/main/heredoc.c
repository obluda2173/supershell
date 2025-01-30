/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:00:40 by erian             #+#    #+#             */
/*   Updated: 2025/01/30 09:48:30 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

volatile sig_atomic_t	signal_received = 0;

void	handle_signals_heredoc(int signum)
{
	(void)signum;
	ft_putendl_fd("", STDOUT_FILENO);
	exit(EXIT_FAILURE);
}

void child_heredoc(int pipefd[2]) {
	signal(SIGINT, handle_signals_heredoc);
	close(pipefd[0]);

	char* line= readline("heredoc> ");
	if (line == NULL)
	{
		ft_putendl_fd("warning: heredoc delimited by end-of-file", STDOUT_FILENO);
		close(pipefd[1]); /* Reader will see EOF */
		exit(EXIT_SUCCESS);
	}
	write(pipefd[1], line, ft_strlen(line) + 1);
	close(pipefd[1]); /* Reader will see EOF */
	free(line);
	exit(EXIT_SUCCESS);
}

char	*read_heredoc_input(char *delimiter, t_data *data)
{
	char	*line;
	char	*heredoc_input;
	char	*tmp;

	heredoc_input = ft_strdup("");
	while (1)
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
			child_heredoc(pipefd);
		close(pipefd[1]);
		wait(NULL);
		if (signal_received == 1)
		{
			data->exit_status = 130;
			signal_received = 0;
			free(heredoc_input);
			return NULL;
		}
		line = read_line_from_fd(pipefd[0]);
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		tmp = ft_strjoin(heredoc_input, line);
		free(heredoc_input);
		heredoc_input = ft_strjoin(tmp, "\n");
		free(tmp);
		free(line);
		if (!heredoc_input)
		{
			printf("Error: Memory allocation failed 1.\n");
			return (NULL);
		}
	}
	return (heredoc_input);
}
bool is_quoted_delimiter(char* delimiter) {
	if (ft_strchr(delimiter, '\''))
		return true;
	if (ft_strchr(delimiter, '\"'))
		return true;
	return false;
}

int count_non_quotes(char *str) {
	int count = 0;
	if (!str)
		return count;
	while (*str) {
		if (!(*str == '\'' || *str == '\"'))
			count++;
		str++;
	}
	return count;
}

char *remove_quotes(char *str) {
	if (!str)
		return str;
	if (*str == '\0')
		return str;

	int final_len = count_non_quotes(str);
	char* new_str = malloc(sizeof(char) * (final_len + 1));
	if (!new_str) {
		ft_putendl_fd("Allocation error", STDERR_FILENO);
		return NULL;
	}
	new_str[final_len] = '\0';
	int i = 0;
	while (*str) {
		if (!(*str == '\'' || *str == '\"'))
			new_str[i++] = *str;
		str++;
	}
	return new_str;
}

int	heredoc_loop(t_dllist **tokens, t_data *data)
{
	char		*delimiter;
	char		*heredoc_input;
	t_dllist	*new_token_node;
	t_dllist	*heredoc_token;
	bool quoted_delimiter;
	char* new_delimiter;

	heredoc_token = search_heredoc(*tokens);
	if (!heredoc_token)
		return EXIT_SUCCESS;
	delimiter = extract_delimiter(&heredoc_token);
	if (!delimiter)
		return EXIT_FAILURE;
	quoted_delimiter = is_quoted_delimiter(delimiter);
	if (quoted_delimiter) {
		new_delimiter = remove_quotes(delimiter);
		free(delimiter);
		delimiter = new_delimiter;
	}
	heredoc_input = read_heredoc_input(delimiter, data);
	free(delimiter);
	if (!heredoc_input)
		return EXIT_FAILURE;
	new_token_node = create_heredoc_token(heredoc_token, heredoc_input, quoted_delimiter);
	if (!new_token_node)
		return EXIT_FAILURE;
	if (heredoc_token->next)
	{
		new_token_node->next = heredoc_token->next;
		new_token_node->next->prev = new_token_node;
	}
	else
		new_token_node->next = NULL;
	new_token_node->prev = heredoc_token->prev;
	heredoc_token->prev->next = new_token_node;
	ft_dllstdelone(heredoc_token, free_token);
	return heredoc_loop(&(new_token_node->next), data);
}
