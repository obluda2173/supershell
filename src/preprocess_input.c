/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:33:18 by erian             #+#    #+#             */
/*   Updated: 2025/01/07 13:48:28 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Handle numbers preceding `>` or `>>`
static char	*handle_redirection(char *input, size_t *i, char *result, size_t *j)
{
	while (ft_isdigit(input[*i]))
		result[(*j)++] = input[(*i)++];
	if (input[*i] == '>')
	{
		result[(*j)++] = input[(*i)++];
		if (input[*i] == '>')
			result[(*j)++] = input[(*i)++];
	}
	if (input[*i] && input[*i] != ' ')
		result[(*j)++] = ' ';
	return (result);
}

// Handle heredoc `<<` without space separation
static char	*handle_heredoc(char *input, size_t *i, char *result, size_t *j)
{
	while (input[*i] == '<')
		result[(*j)++] = input[(*i)++];
	while (input[*i] && !isspace(input[*i]))
		result[(*j)++] = input[(*i)++];
	if (input[*i] && input[*i] != ' ')
		result[(*j)++] = ' ';
	return (result);
}

// Handle quotes
static char	*handle_quotes(const char *input, size_t *i, char *result, size_t *j)
{
	char quote;

	quote = input[(*i)++];
	result[(*j)++] = quote;
	while (input[*i] && input[*i] != quote)
		result[(*j)++] = input[(*i)++];
	if (input[*i])
		result[(*j)++] = input[(*i)++];
	return (result);
}

char *handle_operators(const char *input, size_t *i, char *result, size_t *j)
{
    char *operators[] = {">>", "<<", "||", "&&", "|", "<", ">", "="};
    size_t op_len;
    size_t k;
    size_t l;

    op_len = 0;
    k = -1;
    while (++k < 8)
	{
        op_len = strlen(operators[k]);
        if (strncmp(&input[*i], operators[k], op_len) == 0) 
		{
            l = -1;
            if (*j > 0 && result[*j - 1] != ' ')
                result[(*j)++] = ' ';
            while (++l < op_len)
                result[(*j)++] = input[(*i)++];
            if (input[*i] && input[*i] != ' ')
                result[(*j)++] = ' ';
            return result;
        }
    }
    result[(*j)++] = input[(*i)++];
    return (result);
}

char *preprocess_input(char *input)
{
    size_t len = strlen(input);
    char *result = malloc(len * 2 + 1);
    size_t i = 0, j = 0;

    if (!result) return NULL;
    while (input[i])
    {
        if (isdigit(input[i]) && (input[i + 1] == '>' || (input[i + 1] == '>' && input[i + 2] == '>')))
            result = handle_redirection(input, &i, result, &j);
        else if (input[i] == '<' && input[i + 1] == '<')
			result = handle_heredoc(input, &i, result, &j);
        else if (input[i] == '\'' || input[i] == '\"')
			result = handle_quotes(input, &i, result, &j);
        else 
			result = handle_operators(input, &i, result, &j);
    }
    result[j] = '\0';
    printf("\nLine: %s\n\n", result);
    return (result);
}
