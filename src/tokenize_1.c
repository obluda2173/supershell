/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:42:33 by erian             #+#    #+#             */
/*   Updated: 2024/12/25 16:51:42 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Skips spaces in the input string
void skip_spaces(char *line, int *i)
{
	while (line[*i] && line[*i] == ' ')
		(*i)++;
}

// Tokenizes the input line into a doubly linked list
t_token_list *tokenize(char *line)
{
	int i;
	int operator;
	t_token_list *head;
	t_token_list *prev;
	t_token_list *current;

	i = 0;
	head = NULL;
	prev = NULL;
	if (!line)
		return (NULL);
	skip_spaces(line, &i);
	while (line[i])
	{
		operator = identify_operator(line, i);
		if (operator)
			current = create_operator_token(line, &i, operator);
		else
			current = create_command_token(line, &i);
		if (!current)
		{
			free_tokens(head);
			return (NULL);
		}
		link_tokens(&head, current, prev);
		prev = current;
		skip_spaces(line, &i);
	}
	return (head);
}
