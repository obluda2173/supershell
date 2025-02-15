/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokenize_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:06:20 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 13:28:57 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	skip_operator(t_line_container *lc)
{
	if (lc->line[lc->pos] == '<' && lc->line[lc->pos + 1] == '<'
		&& lc->line[lc->pos + 2] != ' ')
	{
		lc->pos += 2;
		while (lc->line[lc->pos] && lc->line[lc->pos] != ' ')
			lc->pos++;
	}
	else if (lc->line[lc->pos + 1] == lc->line[lc->pos])
		lc->pos += 2;
	else
		lc->pos++;
}

void	skip_word(t_line_container *lc)
{
	char	quote;

	while (lc->line[lc->pos])
	{
		if (lc->line[lc->pos] == '\'' || lc->line[lc->pos] == '\"')
		{
			quote = lc->line[lc->pos];
			lc->pos++;
			while (lc->line[lc->pos] && lc->line[lc->pos] != quote)
				lc->pos++;
			if (lc->line[lc->pos] == quote)
				lc->pos++;
		}
		else if (ft_strchr(" <>|&()", lc->line[lc->pos]))
			break ;
		else
			lc->pos++;
	}
}

char	*allocate_word(const char *line, int start, size_t len)
{
	char	*word;

	word = malloc(len + 1);
	if (!word)
	{
		printf("Error: Memory allocation failed.\n");
		return (NULL);
	}
	ft_strlcpy(word, line + start, len + 1);
	return (word);
}
