/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:46:13 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/31 13:09:16 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_var	*new_env_var(char *key, char *value)
{
	t_env_var	*env_var;

	env_var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!env_var)
		return (NULL);
	env_var->key = ft_strdup(key);
	env_var->value = NULL;
	if (value)
		env_var->value = ft_strdup(value);
	return (env_var);
}

void	free_env_var(void *content)
{
	t_env_var	*env_var;

	env_var = (t_env_var *)content;
	free(env_var->value);
	free(env_var->key);
	free(env_var);
}

void	*teardown_data(t_data *data)
{
	free_data(data);
	return (NULL);
}

t_env_var	*extract_env_var(char *ep)
{
	char		*equal_sign;
	char		*key;
	char		*value;
	t_env_var	*env_var;

	equal_sign = ft_strchr(ep, '=');
	if (equal_sign)
	{
		key = ft_substr(ep, 0, equal_sign - ep);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		key = ft_strdup(ep);
		value = ft_strdup("");
	}
	env_var = new_env_var(key, value);
	free(key);
	free(value);
	return (env_var);
}

t_data	*set_env_vars(char **ep, t_data *data)
{
	t_env_var	*env_var;
	t_list		*new_node;

	while (*ep != NULL)
	{
		env_var = extract_env_var(*ep);
		if (!env_var)
			return (teardown_data(data));
		new_node = ft_lstnew(env_var);
		if (!new_node)
			return (teardown_data(data));
		ft_lstadd_back(&(data->ep), new_node);
		ep++;
	}
	return (data);
}
