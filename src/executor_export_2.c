/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:46:03 by erian             #+#    #+#             */
/*   Updated: 2025/01/26 13:48:02 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static bool	valid_key(char *key)
{
	int	i;

	i = 1;
	if (key[0] != '$')
		return (false);
    if (!ft_isalpha(key[i]) && key[i] != '_')
		return (false);
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
        {
            printf("here fails 3\n");
			return (false);
        }
	}
	return (true);
}

static bool assign_key_value(char *raw_var, char **key, char **value)
{
    char *equals_sign = ft_strchr(raw_var, '=');
    if (equals_sign)
    {
        *key = ft_substr(raw_var, 0, equals_sign - raw_var);
        *value = ft_strdup(equals_sign + 1);
    }
    else
    {
        *key = ft_strdup(raw_var);
        *value = ft_strdup("");
    }
    return (*key && *value);
}

static char *sanitize_key(char *key)
{
    char *sanitized_key = ft_strdup(key + 1);
    free(key);
    return sanitized_key;
}

static char *sanitize_value(char *value)
{
    char *trimmed_value;
    if (value[0] == '"' && ft_strlen(value) > 0)
    {
        trimmed_value = ft_strtrim(value, "\"");
        free(value);
        return trimmed_value;
    }
    return value;
}

bool assign_var(t_env_var **new_var, char *raw_var)
{
    char *key;
    char *value;

    if (!assign_key_value(raw_var, &key, &value))
        return false;

    if (!valid_key(key))
    {
        free(key);
        free(value);
        return false;
    }

    (*new_var)->key = sanitize_key(key);
    (*new_var)->value = sanitize_value(value);

    return true;
}
