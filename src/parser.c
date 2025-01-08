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
#include "lexer_kay.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>

t_script_node	*parse_pipe(t_dllist *tokens)
{
	t_script_node	*sn;

	sn = (t_script_node *)malloc(sizeof(t_script_node));
	sn->node_type = PIPE_NODE;
	sn->num_children = 2;

	sn->downstream = parse_cmd(tokens->next);
	if (!sn->downstream)
	{
		free(sn);
		return (get_error_node("could not alloc space for pipe downstream"));
	}
	tokens = tokens->prev;
	ft_dllstclear(&tokens->next, free_token);

	ft_dllstadd_back(&tokens, ft_dllstnew(new_eof_token()));
	while (tokens->prev && ((t_token*)tokens->content)->type != PIPE)
		tokens = tokens->prev;

	sn->upstream = parse(tokens);
	return (sn);
}

t_script_node	*parse_logical(t_dllist *tokens)
{
	t_script_node	*sn;

	sn = (t_script_node *)malloc(sizeof(t_script_node));

	if (((t_token*)tokens->content)->type == AND)
		sn->node_type = AND_NODE;
	if (((t_token*)tokens->content)->type == OR)
		sn->node_type = OR_NODE;
	sn->num_children = 2;

	sn->downstream = parse_cmd(tokens->next);
	if (!sn->downstream)
	{
		free(sn);
		return (get_error_node("could not alloc space for pipe downstream"));
	}
	tokens = tokens->prev;

	ft_dllstclear(&tokens->next, free_token);
	ft_dllstadd_back(&tokens, ft_dllstnew(new_eof_token()));

	while (tokens->prev && ((t_token*)tokens->content)->type != AND)
		tokens = tokens->prev;

	sn->upstream = parse(tokens);
	return (sn);
}

t_dllist *find_last_logical(t_dllist *tokens) {

	tokens = ft_dllstlast(tokens);
	while (tokens->prev)
	{
		if (((t_token *)tokens->content)->type == AND || ((t_token *)tokens->content)->type == OR)
			return tokens;
		tokens = tokens->prev;
	}
	return tokens;
}

t_dllist *find_last_pipe(t_dllist *tokens) {

	tokens = ft_dllstlast(tokens);
	while (tokens->prev)
	{
		if (((t_token *)tokens->content)->type == PIPE)
			return tokens;
		tokens = tokens->prev;
	}
	return tokens;
}

t_script_node	*parse(t_dllist *tokens)
{
	if (!tokens)
		return (get_error_node("no tokens"));

	tokens = find_last_logical(tokens);
	if (((t_token *)tokens->content)->type == AND || ((t_token *)tokens->content)->type == OR)
		return (parse_logical(tokens));

	tokens = find_last_pipe(tokens);
	if (((t_token *)tokens->content)->type == PIPE)
		return (parse_pipe(tokens));

	return (parse_cmd(tokens));
}
