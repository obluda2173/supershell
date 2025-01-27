/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:01/54 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/02 14:01:54 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"

int	check_redirection_token(t_token t)
{
	char	*content;

	content = t.content;
	while (*content != '<' && *content != '>')
		if (ft_isalpha(*(content++)))
			return (-1);
	if (t.content[0] == '>')
		return (1);
	if (t.content[0] != '>' && t.content[0] != '<')
		return (ft_atoi(t.content));
	return (0);
}

t_redirection	*parse_redirection_token(t_token t)
{
	int				fd;
	t_redirection	*r;

	fd = check_redirection_token(t);
	if (fd == -1)
		return (NULL);
	r = (t_redirection *)malloc(sizeof(t_redirection));
	if (!r)
		return (NULL);
	r->fd = fd;
	if (t.type == REDIRECT_OUT)
		r->type = OUT;
	else if (t.type == REDIRECT_IN)
		r->type = IN;
	else if (t.type == REDIRECT_APPEND)
		r->type = APPEND;
	else if (t.type == HERE_DOC)
		r->type = HERED;
	return (r);
}

t_redirection	*parse_redirection_word(t_token t, t_redirection *r)
{
	/* if (r->type == HERED && (t.type != SINGLE_QUOTE && t.type != DOUBLE_QUOTE)) */
	/* if (r->type == HERED && (t.type != SINGLE_QUOTE_HERE_DOC && t.type != DOUBLE_QUOTE_HERE_DOC)) */
	/* { */
	/* 	free(r); */
	/* 	return (NULL); */
	/* } */
	/* if (t.type == WORD || t.type == SINGLE_QUOTE) */
	if (t.type == WORD)
		r->word_type = LITERAL;
	/* else if (t.type == DOUBLE_QUOTE_HERE_DOC) */
	/* 	r->word_type = DOUBLE_QUOTE_STR; */
	/* else if (t.type == SINGLE_QUOTE_HERE_DOC) */
	/* 	r->word_type = SINGLE_QUOTE_STR; */
	/* else if (t.type == DOLLAR) */
	/* 	r->word_type = ENV_EXP; */
	/* else if (t.type == DOUBLE_QUOTE) */
	/* 	r->word_type = DOUBLE_QUOTE_STR; */
	else if (t.type == WILDCARD)
		r->word_type = WILDCARD_EXP;
	else
	{
		free(r);
		return (NULL);
	}
	r->word = ft_strdup(t.content);
	return (r);
}

t_token	copy_token(t_token token)
{
	t_token	copy;

	copy.content = ft_strdup(token.content);
	copy.type = token.type;
	return (copy);
}

bool	is_redirection_token(t_token t)
{
	if (t.type == REDIRECT_OUT || t.type == REDIRECT_IN
		|| t.type == REDIRECT_APPEND || t.type == HERE_DOC)
		return (true);
	return (false);
}
