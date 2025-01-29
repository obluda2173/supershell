/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_paranthesis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:57/34 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 14:57:34 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

/* removes last RPAREN from downstream;
 * throws error if if appears at a wrong place */
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

bool	has_surrounding_parens(t_dllist *tokens)
{
	t_dllist	*scnd_to_last;
	t_dllist	*head;
	int			nbr_open_parens;

	scnd_to_last = ft_dllstlast(tokens)->prev;
	if (((t_token *)tokens->content)->type == LPAREN
		&& ((t_token *)scnd_to_last->content)->type == RPAREN)
	{
		head = tokens->next;
		nbr_open_parens = 1;
		while (head != scnd_to_last)
		{
			if (((t_token *)head->content)->type == LPAREN)
				nbr_open_parens++;
			if (((t_token *)head->content)->type == RPAREN)
				nbr_open_parens--;
			if (nbr_open_parens == 0)
				break ;
			head = head->next;
		}
		if (head == scnd_to_last)
			return (true);
	}
	return (false);
}

t_dllist	*remove_surrounding_parens(t_dllist *tokens)
{
	t_dllist	*scnd_to_last;
	t_dllist	*head;

	scnd_to_last = ft_dllstlast(tokens)->prev;
	head = tokens;
	tokens = head->next;
	tokens->prev = NULL;
	scnd_to_last->prev->next = scnd_to_last->next;
	scnd_to_last->next->prev = scnd_to_last->prev;
	scnd_to_last->next = NULL;
	scnd_to_last->prev = NULL;
	ft_dllstclear(&scnd_to_last, free_token);
	return (tokens);
}
