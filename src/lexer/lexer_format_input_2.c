/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_format_input_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:55:43 by erian             #+#    #+#             */
/*   Updated: 2025/01/15 11:59:53 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char	*handle_redirection(char *input, size_t *i, char *result, size_t *j)
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

char	*handle_heredoc(char *input, size_t *i, char *result, size_t *j)
{
	while (input[*i] == '<')
		result[(*j)++] = input[(*i)++];
	if (input[*i] && !ft_isspace(input[*i]))
        result[(*j)++] = ' ';
	while (input[*i] && !ft_isspace(input[*i]))
		result[(*j)++] = input[(*i)++];
	if (input[*i] && input[*i] != ' ')
		result[(*j)++] = ' ';
	return (result);
}

char	*handle_quotes(const char *input, size_t *i,
							char *result, size_t *j)
{
	char	quote;

	quote = input[(*i)++];
	result[(*j)++] = quote;
	while (input[*i] && input[*i] != quote)
		result[(*j)++] = input[(*i)++];
	if (input[*i])
		result[(*j)++] = input[(*i)++];
	return (result);
}

void	init_operators_list(char **operators)
{
	operators[0] = ">>";
	operators[1] = "<<";
	operators[2] = "||";
	operators[3] = "&&";
	operators[4] = "|";
	operators[5] = ">";
	operators[6] = "<";
	operators[7] = "=";
	operators[8] = "(";
	operators[9] = ")";
}

char	*handle_operators(const char *input, size_t *i,
								char *result, size_t *j)
{
	char	*operators[10];
	size_t	op_len;
	size_t	k;
	size_t	l;

	op_len = 0;
	k = -1;
	init_operators_list(operators);
	while (++k < 10)
	{
		op_len = ft_strlen(operators[k]);
		if (ft_strncmp(&input[*i], operators[k], op_len) == 0)
		{
			l = -1;
			if (*j > 0 && result[*j - 1] != ' ')
				result[(*j)++] = ' ';
			while (++l < op_len)
				result[(*j)++] = input[(*i)++];
			if (input[*i] && input[*i] != ' ')
				result[(*j)++] = ' ';
			return (result);
		}
	}
	result[(*j)++] = input[(*i)++];
	return (result);
}
