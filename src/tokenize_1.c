/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:42:33 by erian             #+#    #+#             */
/*   Updated: 2024/12/29 11:40:56 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Skips spaces in the input string
void skip_spaces(char *line, int *i)
{
	while (line[*i] && line[*i] == ' ')
		(*i)++;
}
 
//extract allocated word from line
static char	*extract_word(t_line_container *lc)
{
	int		start;
	int		len;
	char	*word;

	start = lc->pos;
	len = 0;
	if (lc->line[lc->pos] == '\'' || lc->line[lc->pos] == '\"' ||
		lc->line[lc->pos] == '<' || lc->line[lc->pos] == '>' ||
		lc->line[lc->pos] == '|')
	{
		if ((lc->line[lc->pos] == '<' && lc->line[lc->pos + 1] == '<') ||
			(lc->line[lc->pos] == '>' && lc->line[lc->pos + 1] == '>'))
			lc->pos += 2;
		else
			lc->pos++;
	}
	else
		while (lc->line[lc->pos] && lc->line[lc->pos] != ' ' &&
				lc->line[lc->pos] != '\'' && lc->line[lc->pos] != '\"' &&
				lc->line[lc->pos] != '<' && lc->line[lc->pos] != '>' &&
				lc->line[lc->pos] != '|')
			lc->pos++;
	len = lc->pos - start;
	word = malloc(len + 1);
	ft_strlcpy(word, lc->line + start, len + 1);
	return (word);
}

//returns token of given content
static token_type	assign_type(char *str)
{
	if (strcmp(str, "|") == 0)
		return (PIPE);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (HERE_DOC);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (REDIRECT_APPEND);
	else if (ft_strncmp(str, "<", 1) == 0)
		return (REDIRECT_IN);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (REDIRECT_OUT);
	else if (ft_strncmp(str, "'", 1) == 0)
		return (SINGLE_QUOTE);
	else if (ft_strncmp(str, "\"", 1) == 0)
		return (DOUBLE_QUOTE);
	else if (ft_strlen(str) > 0)
	{
		if (ft_strchr(str, '='))
			return (ARG);
		else
			return (CMD);
	}
	return END_OF_FILE;
}

// Tokenizes the input line into a doubly linked list
t_token	*get_next_token(t_line_container *lc)
{
	t_token	*token;
	char	*word;

	if (!lc->line)
		return NULL;

	skip_spaces((char *)lc->line, &(lc->pos));

	if (lc->line[lc->pos] == '\0')
	{
		token = malloc(sizeof(t_token));
		if (!token)
			return (NULL);
		token->content = NULL;
		token->type = END_OF_FILE;
		return (token);
	}

	word = extract_word(lc);

	token = malloc(sizeof(t_token));
	
	token->content = word;
	token->type = assign_type(word);

	return (token);
}
