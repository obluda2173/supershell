/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 14:25:12 by erian             #+#    #+#             */
/*   Updated: 2024/12/25 16:57:49 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Gets the next token type based on the operator
int identify_operator(char *line, int i)
{
	int operator;

	operator = 0;
	if (line[i] == '|')
		operator = PIPE;
	else if (line[i] == '>')
	{
		operator = REDIRECT_OUT;
		if (line[i + 1] == '>')
			operator = REDIRECT_APPEND;
	}
	else if (line[i] == '<')
	{
		operator = REDIRECT_IN;
		if (line[i + 1] == '<')
			operator = HERE_DOC;
	}
	return (operator);
}

// Creates a new token with the given content and type
t_token *create_token(char *content, token_type type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(content);
		return (NULL);
	}
	token->content = content;
	token->type = type;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

// Creates a token for operators
t_token *create_operator_token(char *line, int *i, int operator)
{
	t_token *token;
	char *content;
	int len;

	len = (operator == REDIRECT_APPEND || operator == HERE_DOC) ? 2 : 1;
	content = ft_strndup(&line[*i], len);
	token = create_token(content, operator);
	*i += len;
	return (token);
}

// Creates a token for commands or arguments
t_token *create_command_token(char *line, int *i)
{
	int start;
	int len;
	char *content;

	skip_spaces(line, i);
	start = *i;
	while (line[*i] && line[*i] != ' ' && identify_operator(line, *i) == 0)
		(*i)++;
	len = *i - start;
	if (len <= 0)
		return (NULL);
	content = ft_strndup(&line[start], len);
    if (!content)
        return (NULL);
    t_token *token = create_token(content, CMD);
    if (!token)
    {
        free(content);
        return (NULL);
    }
    return (token);
}

// Links tokens to form a doubly linked list
void link_tokens(t_token **head, t_token *current, t_token *prev)
{
	if (!current)
		return ;
	current->prev = prev;
	if (prev)
		prev->next = current;
	if (!*head)
		*head = current;
}
