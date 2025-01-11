/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 09:49/51 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/09 09:49:51 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

t_script_node	*parse_cmd(t_dllist *tokens)
{
	t_script_node	*sn;

	if ((*(t_token *)tokens->content).type == END_OF_FILE)
		return (NULL);
	sn = get_cmd_node(*(t_token *)tokens->content);
	if (!sn)
		return (NULL);
	return (fill_cmd_node(sn, tokens));
}

t_script_node	*parse_pipe(t_dllist *tokens)
{
	t_script_node	*sn;

	sn = get_branch_node(PIPE_NODE);
	sn->downstream = parse_cmd(tokens->next);
	if (!sn->downstream)
	{
		free_script_node(sn);
		return (get_error_node("error parsing command after pipe"));
	}
	if (sn->downstream->node_type == ERROR_NODE) {
		t_script_node *err_node = sn->downstream;
		free(sn);
		return err_node;
	}
	tokens = tokens->prev;
	if (!tokens)
	{
		free_script_node(sn);
		return (get_error_node("error parsing command before pipe"));
	}
	ft_dllstclear(&tokens->next, free_token);
	ft_dllstadd_back(&tokens, ft_dllstnew(create_token(NULL, END_OF_FILE)));
	while (tokens->prev && ((t_token *)tokens->content)->type != PIPE)
		tokens = tokens->prev;
	sn->upstream = parse(tokens);
	if (!sn->upstream)
	{
		free_script_node(sn);
		return (get_error_node("error parsing command before pipe"));
	}

	if (sn->upstream->node_type == ERROR_NODE) {
		t_script_node *err_node = sn->upstream;
		free_script_node(sn->downstream);
		free(sn);
		return err_node;
	}
	return (sn);
}

t_script_node	*parse_logical(t_dllist *tokens)
{
	t_script_node	*sn;

	if (((t_token *)tokens->content)->type == AND)
		sn = get_branch_node(AND_NODE);
	if (((t_token *)tokens->content)->type == OR)
		sn = get_branch_node(OR_NODE);
	tokens->next->prev = NULL;
	sn->downstream = parse(tokens->next);
	if (!sn->downstream)
	{
		free(sn);
		return (get_error_node("error parsing pipeline after logical operator"));
	}
	tokens = tokens->prev;
	if (!tokens)
	{
		free_script_node(sn);
		return (get_error_node("error parsing pipeline before logical operator"));
	}

	ft_dllstclear(&tokens->next, free_token);
	ft_dllstadd_back(&tokens, ft_dllstnew(create_token(NULL, END_OF_FILE)));
	while (tokens->prev && ((t_token *)tokens->content)->type != AND)
		tokens = tokens->prev;
	sn->upstream = parse(tokens);
	if (sn->upstream->node_type == ERROR_NODE)
	{
		free_script_node(sn);
		return (get_error_node("error parsing pipeline before logical operator"));
	}
	return (sn);
}
