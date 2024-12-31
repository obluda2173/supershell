/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:42:33 by erian             #+#    #+#             */
/*   Updated: 2024/12/31 17:44:05 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
			(lc->line[lc->pos] == '>' && lc->line[lc->pos + 1] == '>') ||
			(lc->line[lc->pos] == '&' && lc->line[lc->pos + 1] == '&') ||
			(lc->line[lc->pos] == '|' && lc->line[lc->pos + 1] == '|'))
			lc->pos += 2;
		else
			lc->pos++;
	}

	else
	{
		while (lc->line[lc->pos] && lc->line[lc->pos] != ' ' &&
				lc->line[lc->pos] != '\'' && lc->line[lc->pos] != '\"' &&
				lc->line[lc->pos] != '<' && lc->line[lc->pos] != '>' &&
				lc->line[lc->pos] != '|')
			lc->pos++;
	}

	len = lc->pos - start;
	word = malloc(len + 1);
	ft_strlcpy(word, lc->line + start, len + 1);
	printf("%s\n", word);
	return (word);
}

bool	is_builtin(char *str)
{
	if ((ft_strncmp(str, "echo", 4) == 0 && ft_strlen(str) == 4) || 
		(ft_strncmp(str, "cd", 2) == 0 && ft_strlen(str) == 2) || 
		(ft_strncmp(str, "pwd", 3) == 0 && ft_strlen(str) == 3)|| 
		(ft_strncmp(str, "export", 6) == 0 && ft_strlen(str) == 6)|| 
		(ft_strncmp(str, "unset", 5) == 0 && ft_strlen(str) == 5)|| 
		(ft_strncmp(str, "env", 3) == 0 && ft_strlen(str) == 3)|| 
		(ft_strncmp(str, "exit", 4) == 0 && ft_strlen(str) == 4))
		return (true);
	return (false);
}

//returns token of given content
static token_type	assign_type(char *str)
{
	printf("here is assign type: %s\n", str);
	if (ft_strncmp(str, "||", 2) == 0)
		return (OR);
	else if (ft_strncmp(str, "&&", 2) == 0)
		return (AND);
	else if (ft_strncmp(str, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(str, "$", 1) == 0)
		return (DOLLAR);
	else if (ft_strchr(str, '*') != 0)
		return (WILDCARD);
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
		if (is_builtin(str))
			return (BUILTIN);
		else
			return (WORD);
		//implement invalid
	}
	return END_OF_FILE;
}

// Tokenizes the input line into a doubly linked list
t_token	*get_next_token(t_line_container *lc)
{
	t_token	*token;
	char	*word;
	char	*if_dollar;

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
	
	token->type = assign_type(word);
	if (token->type == DOLLAR)
	{
		if_dollar = ft_substr(word, 1, ft_strlen(word));
		token->content = if_dollar;
		free(word);
	}
	else
		token->content = word;

	return (token);
}
