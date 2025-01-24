/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_format_input_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:33:18 by erian             #+#    #+#             */
/*   Updated: 2025/01/13 14:06:04 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <unistd.h>

static char	*trim_spaces(char *str)
{
	const char *start;
	const char *end;
	size_t trimmed_length;
	char *trimmed_str;

	if (!str)
		return (NULL);
	end = str + ft_strlen(str) - 1;
	start = str;
	while (*start && ((*start >= 9  && *start <= 13) || *start == 32))
		start++;
	while (end > start && ((*end >= 9  && *end <= 13) || *end == 32))
		end--;
	trimmed_length = end - start + 1;
	trimmed_str = malloc(trimmed_length + 1);
	if (!trimmed_str)
		return (NULL);
	ft_strlcpy(trimmed_str, start, trimmed_length + 1);
	free(str);
	return (trimmed_str);
}

char	*format_input(char *input)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*result;

	len = ft_strlen(input);
	i = 0;
	j = 0;
	result = malloc(len * 2 + 1);
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (ft_isdigit(input[i]) && (input[i + 1] == '>'
				|| (input[i + 1] == '>' && input[i + 2] == '>')))
			result = handle_redirection(input, &i, result, &j);
		else if (input[i] == '<' && input[i + 1] == '<')
			result = handle_heredoc(input, &i, result, &j);
		else if (input[i] == '\'' || input[i] == '\"')
			result = handle_quotes(input, &i, result, &j);
		else
			result = handle_operators(input, &i, result, &j);
	}
	result[j] = '\0';
	return (trim_spaces(result));
}
