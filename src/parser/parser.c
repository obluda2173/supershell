/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:57/10 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/02 13:57:10 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>

t_dllist	*find_last_logical(t_dllist *tokens)
{
	tokens = ft_dllstlast(tokens);
	while (tokens->prev)
	{
		if (((t_token *)tokens->content)->type == RPAREN) {
			while (tokens->prev && ((t_token *)tokens->content)->type != LPAREN)
				tokens = tokens->prev;
			continue;
		}
		if (((t_token *)tokens->content)->type == AND
			|| ((t_token *)tokens->content)->type == OR)
			return (tokens);
		tokens = tokens->prev;
	}
	return (tokens);
}

t_dllist	*find_last_pipe(t_dllist *tokens)
{
	tokens = ft_dllstlast(tokens);
	while (tokens->prev)
	{
		if (((t_token *)tokens->content)->type == PIPE)
			return (tokens);
		tokens = tokens->prev;
	}
	return (tokens);
}

t_script_node	*parse(t_dllist *tokens)
{
	if (!tokens)
		return (get_error_node("no tokens"));
	if (((t_token *)ft_dllstlast(tokens)->content)->type != END_OF_FILE)
		return (get_error_node("no end of file token"));
	tokens = find_last_logical(tokens);
	if (((t_token *)tokens->content)->type == AND
		|| ((t_token *)tokens->content)->type == OR)
		return (parse_logical(tokens));
	tokens = find_last_pipe(tokens);
	if (((t_token *)tokens->content)->type == PIPE)
		return (parse_pipe(tokens));
	return (parse_cmd(tokens));
}
