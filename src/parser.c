/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2024/12/28 18:23:50 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//function to print the content of doubly list of tokens
static void print_tokens(t_dllist *head)
{
	t_dllist *current;
	t_token *token;

	if (!head)
	{
		printf("The token list is empty.\n");
		return;
	}
	printf("check1\n");
	while (head && head->prev)
		head = head->prev;

	current = head;
	printf("Tokens in the doubly linked list:\n");
	printf("check2\n");
	while (current && current->content)
	{
		if (!current->content)
		{
			printf("Corrupted token node detected.\n");
			break;
		}
		token = (t_token *)current->content;
		if (token)
		{
			printf("Content: %s, Type: %s\n", token->content, token_strings[token->type]);
		}
		current = current->next;
		printf("check3\n");
	}
}

//main parser
// TODO: this is more the lexer, which seperates text into tokens
// should only be concerned with a char* and should not know about the t_data type
// (inside the function you only work on line and then assign tokens to data->tokens, but this can also be handled by the caller)
// suggestion: call it tokenize, and take only a char*
void tokenize_line(t_dllist **token_list, const char *line)
{
	t_line_container lc = {line, 0};
	t_token *token = NULL;
	t_dllist *new_token_node = NULL;

	while (1)
	{
		token = get_next_token(&lc);
		if (!token)
		{
			printf("Tokenization error or empty line\n");
			return; // Handle memory allocation failure
		}

		new_token_node = ft_dllstnew((void *)token);
		if (!new_token_node)
		{
			printf("Memory allocation failed for token node\n");
			free(token);
			return; // Handle memory failure
		}
		ft_dllstadd_back(token_list, new_token_node);
		if (token->type == END_OF_FILE)
			break;
	}
}

void parse(t_data **data)
{
	if (!data || !*data)
		return;

	(*data)->line = space_line((*data)->line);
	if (!(*data)->line)
		return;

	tokenize_line(&(*data)->tokens, (*data)->line);

	print_tokens((*data)->tokens);
}

