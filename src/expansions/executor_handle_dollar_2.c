/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_dollar_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:46:52 by erian             #+#    #+#             */
/*   Updated: 2025/01/29 17:47:49 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*handle_exit_status(size_t *i, t_data *data)
{
	char	*exit_status_str;

	exit_status_str = ft_itoa(data->exit_status);
	(*i)++;
	return (exit_status_str);
}

char	*handle_env_var(size_t *i, const char *word, t_data *data)
{
	return (expand_variable(word, i, data));
}

char	*handle_other_char(size_t *i, const char *word)
{
	char	*str;

	str = malloc(2);
	if (!str)
		return (NULL);
	str[0] = word[*i];
	str[1] = '\0';
	(*i)++;
	return (str);
}

char	*append_and_check(char *result, size_t *result_len, char *str)
{
	char	*new_result;
	size_t	str_len;

	if (!str)
		return (NULL);
	str_len = ft_strlen(str);
	new_result = append_to_result(result, result_len, str, str_len);
	free(str);
	return (new_result);
}
