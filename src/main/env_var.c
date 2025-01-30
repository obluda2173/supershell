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
