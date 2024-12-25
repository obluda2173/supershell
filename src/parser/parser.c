/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2024/12/25 16:58:45 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//function to print the content of doubly list of tokens
void print_tokens(t_token *head)
{
	t_token *current = head;

	if (!current)
	{
		printf("The token list is empty.\n");
		return;
	}

	printf("Tokens in the doubly linked list:\n");
	while (current)
	{
		printf("Content: %s, Type: %d\n", current->content, current->type);
		current = current->next;
	}
}

//main parser
void	parse(t_data **data)
{
	if (!data || !*data)
		return ;
	(*data)->line = space_line((*data)->line);
	if (!(*data)->line)
	{
		free_all(data);
		return ;
	}
	if ((*data)->line && (*data)->line[0] == '$')
		(*data)->line[0] = (char)(-(*data)->line[0]);
	//printf("%s\n", (*data)->line);
	(*data)->tokens = get_token((*data)->line);
	if (!(*data)->tokens)
	{
		free((*data)->line);
		free_all(data);
		return ;
	}
	//print_tokens((*data)->tokens);
}
