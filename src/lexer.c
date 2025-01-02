/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 17:07:34 by erian             #+#    #+#             */
/*   Updated: 2024/12/31 17:09:00 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer.h"

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
    "INV"
};

void	free_token(void *content)
{
	t_token *token;

	token = (t_token *)content;
	free(token->content);
	free(token);
}

//function to print the content of doubly list of tokens
void print_tokens(t_dllist *head)
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
		{
			printf("Content: %s, Type: %s\n", token->content, token_strings[token->type]);
		}
		current = current->next;
	}
}

t_dllist *tokenize_line(const char *line)
{
	t_line_container lc = {line, 0};
	t_dllist *token_list = NULL;
	t_token *token = NULL;
	t_dllist *new_token_node = NULL;

	while (1)
	{
		token = get_next_token(&lc);
		if (!token)
		{
			printf("Tokenization error or empty line\n");
			return NULL;
		}

		new_token_node = ft_dllstnew((void *)token);
		if (!new_token_node)
		{
			printf("Memory allocation failed for token node\n");
			free(token);
			return NULL;
		}
		ft_dllstadd_back(&token_list, new_token_node);
		if (token->type == END_OF_FILE)
			break ;
	}
	return token_list;
}

t_dllist *tokenize(char* line)
{
	line = space_line(line);
	if (!line)
		return NULL;

	t_dllist *tokens = tokenize_line(line);
	free(line);

	/* print_tokens(tokens); */
	return tokens;
}
