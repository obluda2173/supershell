/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:00:40 by erian             #+#    #+#             */
/*   Updated: 2025/01/15 13:24:56 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <signal.h>
#include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <unistd.h>


volatile sig_atomic_t	signal_received = 0;

void	handle_signals_heredoc(int signum)
{
	(void)signum;
	ft_putendl_fd("", STDOUT_FILENO);
	exit(EXIT_FAILURE);
}


static t_dllist	*search_heredoc(t_dllist *tokens)
{
	t_token		*token_content;
	t_dllist	*token_pointer;

	token_pointer = tokens;
	while (token_pointer)
	{
		token_content = (t_token *)token_pointer->content;
		if (token_content->type == HERE_DOC)
			return (token_pointer);
		token_pointer = token_pointer->next;
	}
	return (NULL);
}

t_dllist	*create_heredoc_token(t_dllist *heredoc_token, char *heredoc_input)
{
	t_token		*token;
	t_dllist	*new_token_node;

	token = (t_token *)heredoc_token->content;
	if (ft_strchr(token->content, '\'') || token->type == SINGLE_QUOTE)
		new_token_node = ft_dllstnew(create_token(heredoc_input, SINGLE_QUOTE));
	else
		new_token_node = ft_dllstnew(create_token(heredoc_input, DOUBLE_QUOTE));
	if (!new_token_node)
	{
		printf("Error: Memory allocation failed 2.\n");
		return (NULL);
	}
	return (new_token_node);
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
		if (cpid == 0) {
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
		close(pipefd[1]);
		wait(NULL);
		if (signal_received == 1)
		{
			data->exit_status = 130;
			signal_received = 0;
			free(heredoc_input);
			return NULL;
		}
		line = read_line_from_child(pipefd[0]);
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
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

char	*extract_delimiter(t_dllist **heredoc_token)
{
	char	*delimiter;
	/* t_token	*token; */
	t_token	*next_token;

	if (!heredoc_token || !*heredoc_token)
		return (NULL);
	delimiter = NULL;
	/* token = (t_token *)(*heredoc_token)->content; */

	if ((*heredoc_token)->next)
	{
		next_token = (t_token *)(*heredoc_token)->next->content;
		fflush(stdout);
		if (next_token->type == WORD || next_token->type == SINGLE_QUOTE)
		{
			delimiter = ft_strdup(next_token->content);
			*heredoc_token = (*heredoc_token)->next;
		}
	}
	if (!delimiter)
	{
		printf("Error: Memory allocation failed 3.\n");
		return (NULL);
	}
	return (delimiter);
}

int	heredoc_loop(t_dllist **tokens, t_data *data)
{
	char		*delimiter;
	char		*heredoc_input;
	t_dllist	*new_token_node;
	t_dllist	*heredoc_token;

	heredoc_token = search_heredoc(*tokens);
	if (!heredoc_token)
		return EXIT_SUCCESS;

	delimiter = extract_delimiter(&heredoc_token);
	if (!delimiter)
		return EXIT_FAILURE;
	heredoc_input = read_heredoc_input(delimiter, data);
	free(delimiter);
	if (!heredoc_input)
		return EXIT_FAILURE;
	new_token_node = create_heredoc_token(heredoc_token, heredoc_input);
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
