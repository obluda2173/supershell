/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debuging_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:16:10 by erian             #+#    #+#             */
/*   Updated: 2025/01/07 15:18:17 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//global function to help print out tokens 
const char *token_strings[] = {
	"BUILTIN",
	"WORD",
	"'",
	"\"",
	"<",
	">",
	"|",
	"<<",
	">>",
	"END",
	"$",
	"&&",
	"||",
	"*",
	"=",
	"INV"
};

//function to print the content of doubly list of tokens
void    print_tokens(t_dllist *head)
{
	t_dllist *current;
	t_token *token;

	if (!head)
	{
		printf("The token list is empty.\n");
		return;
	}
	current = head;
	printf("Tokens in the doubly linked list:\n");
	while (current && current->content)
	{
		if (!current->content)
		{
			printf("Corrupted token node detected.\n");
			break;
		}
		token = (t_token *)current->content;
		if (token)
			printf("Content: %s, Type: %s\n", token->content, token_strings[token->type]);
		current = current->next;
	}
}
