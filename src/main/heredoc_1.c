/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:26/18 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 12:26:18 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include <unistd.h>

t_dllist	*search_heredoc(t_dllist *tokens)
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

char	*add_quotes(char *str)
{
	size_t	len;
	char	*quoted_str;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	quoted_str = malloc(len + 3);
	if (!quoted_str)
	{
		ft_putendl_fd("Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}
	quoted_str[0] = '\'';
	ft_strlcpy(quoted_str + 1, str, len + 2);
	quoted_str[len + 1] = '\'';
	quoted_str[len + 2] = '\0';
	return (quoted_str);
}

char	*add_double_quotes(char *str)
{
	size_t	len;
	char	*quoted_str;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	quoted_str = malloc(len + 3);
	if (!quoted_str)
	{
		ft_putendl_fd("Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}
	quoted_str[0] = '\"';
	ft_strlcpy(quoted_str + 1, str, len + 2);
	quoted_str[len + 1] = '\"';
	quoted_str[len + 2] = '\0';
	return (quoted_str);
}

t_dllist	*create_heredoc_token(char *heredoc_input, bool quoted_delimiter)
{
	t_dllist	*new_token_node;

	if (quoted_delimiter)
		new_token_node = ft_dllstnew(create_token(heredoc_input,
					HD_SINGLE_QUOTE));
	else
		new_token_node = ft_dllstnew(create_token(heredoc_input,
					HD_DOUBLE_QUOTE));
	if (!new_token_node)
	{
		printf("Error: Memory allocation failed 2.\n");
		return (NULL);
	}
	return (new_token_node);
}

char	*extract_delimiter(t_dllist **heredoc_token)
{
	char	*delimiter;
	t_token	*next_token;

	if (!heredoc_token || !*heredoc_token)
		return (NULL);
	delimiter = NULL;
	if ((*heredoc_token)->next)
	{
		next_token = (t_token *)(*heredoc_token)->next->content;
		if (next_token->type == WORD)
		{
			delimiter = ft_strdup(next_token->content);
			*heredoc_token = (*heredoc_token)->next;
		}
		else
			return (error_and_return_null("Syntax Error."));
	}
	if (!delimiter)
	{
		printf("Error: Memory allocation failed 3.\n");
		return (NULL);
	}
	return (delimiter);
}
