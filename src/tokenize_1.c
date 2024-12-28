/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:42:33 by erian             #+#    #+#             */
/*   Updated: 2024/12/28 14:06:46 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Skips spaces in the input string
void skip_spaces(char *line, int *i)
{
	while (line[*i] && line[*i] == ' ')
		(*i)++;
}

char	*extract_word(t_line_container *lc)
{
	//todo
}

token_type	assign_type(char *str)
{
	if (strcmp(str, "|") == 0)
		return (PIPE);
	else if (ft_strncmp(str, "<", 2) == 0)
		return (REDIRECT_IN);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (REDIRECT_OUT);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (HERE_DOC);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (REDIRECT_APPEND);
	else if (ft_strncmp(str, "'", 1) == 0)
		return (SINGLE_QUOTE);
	else if (ft_strncmp(str, "\"", 1) == 0)
		return (DOUBLE_QUOTE);
	else if (ft_strlen(str) > 0)
	{
		if (ft_strchr(str, "="))
			return (ARG);
		else
			return (CMD);
	}
	return END_OF_FILE;
}


// Tokenizes the input line into a doubly linked list
t_token	get_next_token(t_line_container *lc)
{
	char	*word;
	t_token	token;

	if (lc->line[lc->pos] == '\0')
	{
		token.content = NULL;
		token.type = END_OF_FILE;
		return (token);
	}

	word = extract_word(lc);

	token.content = word;
	token.type = assing_type(token.content);

	return (token);
}
