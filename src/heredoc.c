/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:00:40 by erian             #+#    #+#             */
/*   Updated: 2025/01/08 12:55:35 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

t_dllist	*search_heredoc(t_dllist *tokens)
{
	t_token  *token_content;
	t_dllist *token_pointer;
	
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
t_token	*new_token(char *content, token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = content;
	token->type = type;
	return (token);
}

void replace_heredoc_token(t_dllist *old_token, t_dllist *new_token)
{
	new_token->next = old_token->next;
	new_token->prev = old_token;
	old_token->next = new_token;
}

t_dllist *create_heredoc_token(char *heredoc_input)
{
	t_dllist *new_token_node;

	new_token_node = ft_dllstnew(new_token(heredoc_input, DOUBLE_QUOTE));
	if (!new_token_node)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	return (new_token_node);
}

char *read_heredoc_input(char *delimiter)
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
			break;
		}
		tmp = ft_strjoin(heredoc_input, line);
		free(heredoc_input);
		heredoc_input = ft_strjoin(tmp, "\n");
		free(tmp);
		free(line);
		if (!heredoc_input)
		{
			printf("Error: Memory allocation failed.\n");
			return (NULL);
		}
	}
	return (heredoc_input);
}

char *extract_delimiter(t_dllist *heredoc_token)
{
	t_token *token_content;
	char	*delimiter;

	token_content = (t_token *)heredoc_token->content;
	delimiter = ft_strdup(token_content->content + 2); // Skip "<<"
	if (!delimiter)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	return (delimiter);
}

int heredoc_loop(t_dllist **tokens)
{
	char	*delimiter;
	char	*heredoc_input;
	t_dllist *new_token_node;
	t_dllist *heredoc_token = search_heredoc(*tokens);

	if (!heredoc_token)
		return (1);
	delimiter = extract_delimiter(heredoc_token);
	if (!delimiter)
		return (0);
	heredoc_input = read_heredoc_input(delimiter);
	free(delimiter);
	if (!heredoc_input)
		return (0);
	new_token_node = create_heredoc_token(heredoc_input);
	if (!new_token_node)
		return (0);
	replace_heredoc_token(heredoc_token, new_token_node);
	return (1);
}
