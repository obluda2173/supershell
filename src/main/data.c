/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:23/59 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/18 18:23:59 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <sys/types.h>
#include <unistd.h>

t_env_var *new_env_var(char *key, char *value) {
	t_env_var *env_var = (t_env_var*)malloc(sizeof(t_env_var));
	if (!env_var)
		return NULL;
	env_var->key = ft_strdup(key);
	env_var->value = NULL;
	if (value)
		env_var->value = ft_strdup(value);
	return env_var;
}

// initialise data structure and extract environment
t_data	*init(char **ep)
{
	t_list	*new_node;
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->ep = NULL;
	data->exit_status = 0;
	data->exit = false;
	data->line = NULL;
	while (*ep != NULL)
	{
		char** key_and_value = ft_split(*ep, '=');
		char* key = key_and_value[0];
		char* value = NULL;

		if (key_and_value[1])
			value = key_and_value[1];
		else
			value = "";

		t_env_var *env_var = new_env_var(key, value);
		if (!env_var) {
			free_data(data);
			return (NULL);
		}
		free_char_array(key_and_value);

		new_node = ft_lstnew(env_var);
		if (!new_node)
		{
			free_data(data);
			return (NULL);
		}
		ft_lstadd_back(&(data->ep), new_node);
		ep++;
	}

	return (data);
}

void free_env_var(void *content) {
	t_env_var *env_var = (t_env_var*)content;
	free(env_var->value);
	free(env_var->key);
	free(env_var);
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->ep)
		ft_lstclear(&(data->ep), free_env_var);
	if (data->line)
		free(data->line);
	free(data);
}
