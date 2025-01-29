/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 13:49:11 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer.h"

static t_dllist	*tokenize_line(const char *line)
{
	t_token				*token;
	t_dllist			*token_list;
	t_dllist			*new_token_node;
	t_line_container	lc;

	token = NULL;
	token_list = NULL;
	new_token_node = NULL;
	lc.line = line;
	lc.pos = 0;
	while (1)
	{
		token = get_next_token(&lc);
		if (!token)
		{
			printf("Tokenization error or empty line\n");
			return (NULL);
		}
		new_token_node = ft_dllstnew((void *)token);
		ft_dllstadd_back(&token_list, new_token_node);
		if (token->type == END_OF_FILE)
			break ;
	}
	return (token_list);
}

void	free_token(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	free(token->content);
	free(token);
}

t_dllist	*tokenize(char *line)
{
	t_dllist	*tokens;

	line = format_input(line);
	if (!line)
		return (NULL);
	tokens = tokenize_line(line);
	free(line);
	return (tokens);
}
