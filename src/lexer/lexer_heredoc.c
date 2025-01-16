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

#include "minishell.h"

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

char	*read_heredoc_input(char *delimiter)
{
	char	*line;
	char	*heredoc_input;
	char	*tmp;

	heredoc_input = ft_strdup("");
	while (1)
	{
		line = readline("heredoc> ");
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

int	heredoc_loop(t_dllist **tokens)
{
	char		*delimiter;
	char		*heredoc_input;
	t_dllist	*new_token_node;
	t_dllist	*heredoc_token;

	heredoc_token = search_heredoc(*tokens);
	if (!heredoc_token)
		return (1);
	delimiter = extract_delimiter(&heredoc_token);
	if (!delimiter)
		return (0);
	heredoc_input = read_heredoc_input(delimiter);
	free(delimiter);
	if (!heredoc_input)
		return (0);
	new_token_node = create_heredoc_token(heredoc_token, heredoc_input);
	if (!new_token_node)
		return (0);
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
	return (1);
}
