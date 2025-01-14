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
#include "libft.h"
#include "parser.h"
#include <unistd.h>

t_script_node	*treat_parens(t_dllist *tokens)
{
	t_dllist	*head;

	head = tokens;
	head = head->next;
	if ((*(t_token *)head->content).type == RPAREN)
		return (get_error_node("parsing error near ("));
	while ((*(t_token *)head->content).type != RPAREN)
		head = head->next;
	if (((*(t_token *)head->next->content).type != END_OF_FILE))
		return (get_error_node("parsing error near )"));
	head = head->prev;
	ft_dllstclear(&head->next, free_token);
	ft_dllstadd_back(&tokens, ft_dllstnew(create_token(NULL, END_OF_FILE)));
	return (NULL);
}

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

/* removes last RPAREN from downstream; throws error if if appears at a wrong place */
t_script_node	*remove_next_rparen(t_dllist *downstream, t_script_node *sn)
{
	t_dllist	*head;

	if (((t_token *)downstream->content)->type == RPAREN)
		return (teardown(sn, "parsing error near ("));
	head = downstream;
	while (((t_token *)head->content)->type != RPAREN)
		head = head->next;
	if (((t_token *)head->prev->content)->type == PIPE)
		return (teardown(sn, "parsing error near |"));
	if (((t_token *)head->prev->content)->type == AND)
		return (teardown(sn, "parsing error near &&"));
	if (((t_token *)head->prev->content)->type == OR)
		return (teardown(sn, "parsing error near ||"));
	head->next->prev = head->prev;
	head->prev->next = head->next;
	free_token(head->content);
	free(head);
	return (NULL);
}

t_script_node *parse_upstream(t_script_node *sn, t_dllist *tokens) {
	sn->upstream = parse(tokens);
	if (sn->upstream->node_type == ERROR_NODE)
		return (teardown_err_in_upstream(sn));
	if (!sn->upstream)
		return (teardown(sn, "error parsing pipeline before logical operator"));
	return sn;
}

t_script_node *parse_downstream(t_script_node *sn, t_dllist *downstream) {
	sn->downstream = parse(downstream);
	if (!sn->downstream)
		return (teardown(sn, "error parsing pipeline after logical operator"));
	if (sn->downstream->node_type == ERROR_NODE)
		return (teardown_err_in_downstream(sn));
	return sn;
}

t_script_node	*parse_logical(t_dllist *tokens)
{
	t_script_node	*sn;
	t_dllist		*downstream;
	t_script_node	*err;

	if (((t_token *)tokens->content)->type == AND)
		sn = get_branch_node(AND_NODE);
	if (((t_token *)tokens->content)->type == OR)
		sn = get_branch_node(OR_NODE);
	if (((t_token *)tokens->next->content)->type == LPAREN)
	{
		tokens->next->next->prev = NULL;
		downstream = tokens->next->next;
		err = remove_next_rparen(downstream, sn);
		if (err)
			return (err);
	}
	else
	{
		tokens->next->prev = NULL;
		downstream = tokens->next;
	}
	sn = parse_downstream(sn, downstream);
	if (sn->node_type == ERROR_NODE)
		return sn;

	tokens = tokens->prev;
	if (!tokens)
		return (teardown(sn, "error parsing pipeline before logical operator"));
	ft_dllstclear(&tokens->next, free_token);
	ft_dllstadd_back(&tokens, ft_dllstnew(create_token(NULL, END_OF_FILE)));
	while (tokens->prev)
		tokens = tokens->prev;
	return parse_upstream(sn, tokens);
}
