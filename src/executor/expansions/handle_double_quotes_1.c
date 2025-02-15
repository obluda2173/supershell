/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_double_quotes_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 21:40/31 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 21:40:31 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*process_exit_status(t_data *data, char **result, size_t *i)
{
	char	*exit_status_str;
	size_t	exit_status_len;

	exit_status_str = ft_itoa(data->exit_status);
	if (!exit_status_str)
		return (NULL);
	exit_status_len = ft_strlen(exit_status_str);
	*result = append_to_result(*result, exit_status_str, exit_status_len);
	free(exit_status_str);
	if (!*result)
		return (NULL);
	(*i)++;
	return (*result);
}

char	*process_dollar_var(char *dollar_expansion, char **result)
{
	size_t	expansion_len;

	if (!dollar_expansion)
		return (NULL);
	expansion_len = ft_strlen(dollar_expansion);
	*result = append_to_result(*result, dollar_expansion, expansion_len);
	free(dollar_expansion);
	if (!*result)
		return (NULL);
	return (*result);
}

char	*process_normal_char(const char *word, size_t *i, char **result)
{
	char	temp[2];

	temp[0] = word[(*i)++];
	temp[1] = '\0';
	*result = append_to_result(*result, temp, 1);
	return (*result);
}

char	*handle_dollar_expansion(const char *word, t_data *data, size_t *i,
		char *result)
{
	char	*dollar_expansion;

	dollar_expansion = handle_dollar(&word[*i], data);
	result = process_dollar_var(dollar_expansion, &result);
	while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
		*i += 1;
	return (result);
}
