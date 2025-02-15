/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_dollar_1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:46:12 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 17:49:21 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*process_char(const char *word, size_t *i, t_data *data)
{
	if (word[*i] == '?')
		return (handle_exit_status(i, data));
	else if (ft_isalnum(word[*i]) || word[*i] == '_')
		return (handle_env_var(i, word, data));
	else
		return (handle_other_char(i, word));
}

static char	*process_dollar_expansion(const char *word, t_data *data)
{
	size_t	len;
	size_t	i;
	size_t	result_len;
	char	*result;

	len = ft_strlen(word);
	i = 0;
	result_len = 0;
	result = malloc(1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (i < len)
	{
		result = append_and_check(result, &result_len, process_char(word, &i,
					data));
		if (!result)
			return (NULL);
		if (word[i - 1] == '?' || ft_isalnum(word[i - 1]) || word[i - 1] == '_')
			return (result);
	}
	return (result);
}

char	*handle_dollar(const char *word, t_data *data)
{
	if (!is_env_var_char(*word) && *word != '?')
		return (ft_strdup("$"));
	return (process_dollar_expansion(word, data));
}
