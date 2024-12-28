/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2024/12/28 12:04:26 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//function to print the content of doubly list of tokens
void print_tokens(t_token_list *head)
{
	t_token_list *current = head;

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
// TODO: this is more the lexer, which seperates text into tokens
// should only be concerned with a char* and should not know about the t_data type
// (inside the function you only work on line and then assign tokens to data->tokens, but this can also be handled by the caller)
// suggestion: call it tokenize, and take only a char*
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
		(*data)->line[0] = (char)(-(*data)->line[0]); /* TODO: I don't understand (-$ symbol ?) */
	//printf("%s\n", (*data)->line);
	(*data)->tokens = tokenize((*data)->line);
	if (!(*data)->tokens)
	{
		free((*data)->line);
		free_all(data);
		return ;
	}
	//print_tokens((*data)->tokens);
}

