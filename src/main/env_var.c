/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 09:46/13 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/30 09:46:13 by kfreyer          ###   ########.fr       */
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

t_data	*set_env_vars(char **ep, t_data *data)
{
	char		**key_and_value;
	char		*key;
	char		*value;
	t_env_var	*env_var;
	t_list		*new_node;

	while (*ep != NULL)
	{
		key_and_value = ft_split(*ep, '=');
		key = key_and_value[0];
		if (key_and_value[1])
			value = key_and_value[1];
		else
			value = "";
		env_var = new_env_var(key, value);
		if (!env_var)
			return (teardown_data(data));
		free_char_array(key_and_value);
		new_node = ft_lstnew(env_var);
		if (!new_node)
			return (teardown_data(data));
		ft_lstadd_back(&(data->ep), new_node);
		ep++;
	}
	return (data);
}
