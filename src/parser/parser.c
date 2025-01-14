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

	/* /\* remove surrounding parens *\/ */
	t_dllist* scnd_to_last = ft_dllstlast(tokens)->prev;
	bool remove = false;

	/* printf("type first: %d\n", ((t_token *)tokens->content)->type); */
	/* printf("type scnd_to_last: %d\n", ((t_token*)scnd_to_last->content)->type); */
	if (((t_token *)tokens->content)->type == LPAREN && ((t_token*)scnd_to_last->content)->type == RPAREN) {

		if (tokens->next == scnd_to_last) {
			return get_error_node("parsing error near (");
		}
		t_dllist *head = tokens->next;
		/* head = head->next; */
		int nbr_open_parens = 1;
		while (head != scnd_to_last) {
			if (((t_token *)head->content)->type == LPAREN)
				nbr_open_parens++;
			if (((t_token *)head->content)->type == RPAREN)
				nbr_open_parens--;
			if (nbr_open_parens == 0)
				break;
			head = head->next;
		}
		if (head == scnd_to_last)
			remove = true;
	}

	if (remove) {
		t_dllist *head = tokens;
		tokens = head->next;
		tokens->prev = NULL;

		scnd_to_last->prev->next = scnd_to_last->next;
		scnd_to_last->next->prev = scnd_to_last->prev;
		scnd_to_last->next = NULL;
		scnd_to_last->prev = NULL;
		ft_dllstclear(&scnd_to_last, free_token);
		return parse(tokens);
	}

	tokens = find_last_logical(tokens);
	if (((t_token *)tokens->content)->type == AND
		|| ((t_token *)tokens->content)->type == OR)
		return (parse_logical(tokens));
	tokens = find_last_pipe(tokens);
	if (((t_token *)tokens->content)->type == PIPE)
		return (parse_pipe(tokens));
	return (parse_cmd(tokens));
}
