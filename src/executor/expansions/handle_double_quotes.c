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

static char	*process_exit_status(t_data *data, char **result, size_t *i)
{
	char	*exit_status_str = ft_itoa(data->exit_status);
	size_t	exit_status_len;

	if (!exit_status_str)
		return (NULL);
	exit_status_len = ft_strlen(exit_status_str);
	*result = append_to_result(*result,  exit_status_str, exit_status_len);
	free(exit_status_str);
	if (!*result)
		return (NULL);
	(*i)++;
	return (*result);
}

static char	*process_dollar_var(char *dollar_expansion, char **result)
{
	size_t	expansion_len;

	if (!dollar_expansion)
		return (NULL);
	expansion_len = ft_strlen(dollar_expansion);
	*result = append_to_result(*result,  dollar_expansion, expansion_len);
	free(dollar_expansion);
	if (!*result)
		return (NULL);
	return (*result);
}

static char	*process_normal_char(const char *word, size_t *i, char **result)
{
	char temp[2] = {word[(*i)++], '\0'};
	*result = append_to_result(*result,  temp, 1);
	return (*result);
}

char	*handle_double_quotes(const char *word, t_data *data)
{
	size_t	len;
	char	*result;
	size_t	i;
	char	*dollar_expansion;

	result = malloc(1);
	if (!result)
		return (NULL);
	i = 0;
	len = ft_strlen(word);
	result[0] = '\0';
	
	while (i < len && word[i] != '\"')
	{
		if (word[i] == '$')
		{
			i++;
			if (i < len && word[i] == '?')
				result = process_exit_status(data, &result, &i);
			else
			{
				dollar_expansion = handle_dollar(&word[i], data);
				result = process_dollar_var(dollar_expansion, &result);
				while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
					(i)++;
			}
		}
		else
			result = process_normal_char(word, &i, &result);
		if (!result)
			return (NULL);
	}
	return (result);
}
