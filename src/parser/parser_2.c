/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:55/32 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 14:55:32 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_script_node	*parse_cmd(t_dllist *tokens)
{
	t_script_node	*sn;
	t_script_node	*err;

	if ((*(t_token *)tokens->content).type == END_OF_FILE)
		return (NULL);
	if ((*(t_token *)tokens->content).type == LPAREN)
	{
		err = treat_parens(tokens);
		if (err)
			return (err);
		return (parse_cmd(tokens->next));
	}
	sn = get_cmd_node();
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
		return (teardown(sn, "error parsing command after pipe"));
	if (sn->downstream->node_type == ERROR_NODE)
		return (teardown_err_in_downstream(sn));
	tokens = tokens->prev;
	if (!tokens)
		return (teardown(sn, "error parsing command before pipe"));
	ft_dllstclear(&tokens->next, free_token);
	ft_dllstadd_back(&tokens, ft_dllstnew(create_token(NULL, END_OF_FILE)));
	while (tokens->prev && ((t_token *)tokens->content)->type != PIPE)
		tokens = tokens->prev;
	sn->upstream = parse(tokens);
	if (!sn->upstream)
		return (teardown(sn, "error parsing command before pipe"));
	if (sn->upstream->node_type == ERROR_NODE)
		return (teardown_err_in_upstream(sn));
	return (sn);
}

t_script_node	*parse_upstream(t_script_node *sn, t_dllist *tokens)
{
	sn->upstream = parse(tokens);
	if (sn->upstream->node_type == ERROR_NODE)
		return (teardown_err_in_upstream(sn));
	if (!sn->upstream)
		return (teardown(sn, "error parsing pipeline before logical operator"));
	return (sn);
}

t_script_node	*parse_downstream(t_script_node *sn, t_dllist *downstream)
{
	sn->downstream = parse(downstream);
	if (!sn->downstream)
		return (teardown(sn, "error parsing pipeline after logical operator"));
	if (sn->downstream->node_type == ERROR_NODE)
		return (teardown_err_in_downstream(sn));
	return (sn);
}

t_script_node	*parse_logical(t_dllist *tokens)
{
	t_script_node	*sn;
	t_dllist		*downstream_tokens;
	t_script_node	*err;

	if (((t_token *)tokens->content)->type == AND)
		sn = get_branch_node(AND_NODE);
	if (((t_token *)tokens->content)->type == OR)
		sn = get_branch_node(OR_NODE);
	err = set_downstream_tokens(&downstream_tokens, tokens, sn);
	if (err)
		return (err);
	sn = parse_downstream(sn, downstream_tokens);
	if (sn->node_type == ERROR_NODE)
		return (sn);
	err = truncate_tokens(sn, &tokens);
	if (err)
		return (err);
	tokens = ft_dllstfirst(tokens);
	return (parse_upstream(sn, tokens));
}
