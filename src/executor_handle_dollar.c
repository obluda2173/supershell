/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_dollar.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:46:12 by erian             #+#    #+#             */
/*   Updated: 2025/01/12 13:03:15 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*handle_dollar(const char *word, t_data *data)
{
	size_t len = ft_strlen(word);
	char *result = malloc(1);
	size_t result_len = 0;
	size_t i = 0;

	if (!result)
		return NULL;

	result[0] = '\0';
	while (i < len)
	{
		if (word[i] == '?')
		{
			char *exit_status_str = ft_itoa(data->exit_status);
			size_t exit_status_len = ft_strlen(exit_status_str);
			result = append_to_result(result, &result_len, exit_status_str, exit_status_len);
			free(exit_status_str);
			if (!result)
				return NULL;
			i++;
		}
		else if (ft_isalnum(word[i]) || word[i] == '_')
		{
			char *var_value = expand_variable(word, &i, data);
			size_t var_value_len = ft_strlen(var_value);
			result = append_to_result(result, &result_len, var_value, var_value_len);
			free(var_value);
			if (!result)
				return NULL;
		}
		else
		{
			char temp[2] = {word[i++], '\0'};
			result = append_to_result(result, &result_len, temp, 1);
			if (!result)
				return NULL;
		}
	}
	return result;
}
