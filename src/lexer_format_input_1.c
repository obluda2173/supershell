/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_input_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:33:18 by erian             #+#    #+#             */
/*   Updated: 2025/01/09 09:43:22 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (isdigit(input[i]) && (input[i + 1] == '>'
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
	return (result);
}
