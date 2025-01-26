/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:37:32 by erian             #+#    #+#             */
/*   Updated: 2025/01/26 13:43:20 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	valid_key(char *key)
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

bool assign_key_value(char *raw_var, char **key, char **value)
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

char *sanitize_key(char *key)
{
    char *sanitized_key = ft_strdup(key + 1);
    free(key);
    return sanitized_key;
}

char *sanitize_value(char *value)
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

bool add_expansions(t_list **ep, t_list *arg_lst)
{
    t_list      *arg_i;
    t_env_var   *new_var;
    t_argument  *current_arg;

    arg_i = arg_lst;
    while (arg_i)
    {
        current_arg = (t_argument *)arg_i->content;
        
        new_var = malloc(sizeof(t_env_var));
        
        if (!assign_var(&new_var, current_arg->word))
        {
            free(new_var);
            return (false);
        }
        ft_lstadd_back(ep, ft_lstnew(new_var));
        arg_i = arg_i->next;
    }
    return (true);
}

void sort_for_export(t_list **ep)
{
    t_list *tmp_lst, *prev, *last_node;
    t_env_var *env_var;
    int list_size;

    if (!ep || !*ep)
        return;

    sort_arguments(ep);
    tmp_lst = *ep;
    last_node = ft_lstlast(*ep);
    list_size = ft_lstsize(*ep);
    prev = NULL;

    while (list_size > 0 && tmp_lst)
    {
        env_var = (t_env_var *)tmp_lst->content;
        if (!ft_isalpha(env_var->key[0]))
        {
            if (prev)
                prev->next = tmp_lst->next;
            else
                *ep = tmp_lst->next;

            last_node->next = tmp_lst;
            tmp_lst->next = NULL;
            last_node = tmp_lst;

            tmp_lst = (prev) ? prev->next : *ep;
        }
        else
        {
            prev = tmp_lst;
            tmp_lst = tmp_lst->next;
        }
        list_size--;
    }
}

void	print_export(t_list **ep)
{
	t_list	*tmp_ep;
	t_env_var	*env_var;
	
	sort_for_export(ep);
    tmp_ep = *ep;
	while (tmp_ep)
	{
		env_var = (t_env_var *)tmp_ep->content;

        if (env_var->value)
            printf("declare -x %s=\"%s\"\n", env_var->key, env_var->value);
        else
            printf("declare -x %s\n", env_var->key);
        tmp_ep = tmp_ep->next;
	}
}

int	export(t_list **ep, t_cmd_node *cmd_node)
{
	if (!ft_lstsize(cmd_node->arguments))
	{
		print_export(ep);
		return EXIT_SUCCESS;
	}
	
	if (!add_expansions(ep, cmd_node->arguments))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
