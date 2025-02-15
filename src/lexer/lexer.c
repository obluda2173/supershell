/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2025/01/30 17:04:51 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "minishell.h"

static bool	last_token_was_heredoc(t_dllist *token_list)
{
	if (ft_dllstlast(token_list)
		&& ((t_token *)ft_dllstlast(token_list)->content)->type == HERE_DOC)
		return (true);
	return (false);
}

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
			return (error_and_return_null("Tokenization error or empty line"));
		if (last_token_was_heredoc(token_list) && token->type == WILDCARD)
			token->type = WORD;
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
