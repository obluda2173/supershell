/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_double_quotes.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:57:45 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 18:32:22 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*handle_double_quotes(const char *word, t_data *data)
{
	char	*result;
	size_t	i;

	result = malloc(1);
	if (!result)
		return (NULL);
	i = 0;
	result[0] = '\0';
	while (i < ft_strlen(word) && word[i] != '\"')
	{
		if (word[i] == '$')
		{
			i++;
			if (i < ft_strlen(word) && word[i] == '?')
				result = process_exit_status(data, &result, &i);
			else
				result = handle_dollar_expansion(word, data, &i, result);
		}
		else
			result = process_normal_char(word, &i, &result);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*handle_double_quotes_hd(const char *word, t_data *data)
{
	char	*result;
	size_t	i;

	result = malloc(1);
	if (!result)
		return (NULL);
	i = 0;
	result[0] = '\0';
	while (i < ft_strlen(word))
	{
		if (word[i] == '$')
		{
			i++;
			if (i < ft_strlen(word) && word[i] == '?')
				result = process_exit_status(data, &result, &i);
			else
				result = handle_dollar_expansion(word, data, &i, result);
		}
		else
			result = process_normal_char(word, &i, &result);
		if (!result)
			return (NULL);
	}
	return (result);
}
