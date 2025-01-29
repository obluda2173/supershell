/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:47:50 by erian             #+#    #+#             */
/*   Updated: 2025/01/17 12:22:32 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*custom_getenv(const char *key, t_data *data)
{
	t_list		*ep;
	t_env_var	*env_var;

	ep = data->ep;
	while (ep)
	{
		env_var = (t_env_var *)ep->content;
		if (!ft_strcmp(key, env_var->key))
		{
			if (env_var->value)
				return (ft_strdup(env_var->value));
			else
				return (ft_strdup(""));
		}
		ep = ep->next;
	}
	return (NULL);
}

char	*handle_env_expansion(const char *var_name, t_data *data)
{
	char	*value;

	value = custom_getenv(var_name, data);
	if (value)
		return (value);
	free(value);
	return (ft_strdup(""));
}

char	*expand_variable(const char *str, size_t *i, t_data *data)
{
	char	*var_name;
	char	*var_value;
	size_t	start;

	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	var_name = ft_strndup(&str[start], *i - start);
	var_value = handle_env_expansion(var_name, data);
	free(var_name);
	if (var_value)
		return (var_value);
	return (ft_strdup(""));
}

char	*append_to_result(char *result, const char *addition, size_t addition_len)
{
	char	*new_result;

	new_result = malloc(ft_strlen(result) + addition_len + 1);
	if (!new_result)
	{
		if (result)
			free(result);
		return (NULL);
	}
	if (result)
		ft_memcpy(new_result, result, ft_strlen(result));
	ft_memcpy(new_result + ft_strlen(result), addition, addition_len);
	new_result[ft_strlen(result) + addition_len] = '\0';
	free(result);
	return (new_result);
}
