/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arguments_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:59/28 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 19:59:28 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>

t_list	*ignore_wildcard_in_argument(char *dir_path, char *pattern)
{
	char		*full_path;
	t_argument	*new;

	full_path = build_full_path(dir_path, pattern);
	new = (t_argument *)malloc(sizeof(t_argument));
	new->word = full_path;
	new->type = LITERAL;
	return (ft_lstnew(new));
}

t_argument	*get_new_argument(char *entry, char *dir_path)
{
	char		*full_path;
	t_argument	*new;

	full_path = build_full_path(dir_path, entry);
	if (!full_path)
		return (NULL);
	new = (t_argument *)malloc(sizeof(t_argument));
	if (!new)
	{
		free(full_path);
		return (NULL);
	}
	new->word = full_path;
	new->type = LITERAL;
	return (new);
}

t_list	*create_wildcard_arguments(t_list *dir_entries, char *dir_path,
		char *pattern)
{
	t_list	*new_arguments;

	new_arguments = parse_dir_entries(dir_entries, dir_path, pattern);
	if (!new_arguments)
		return (ignore_wildcard_in_argument(dir_path, pattern));
	return (new_arguments);
}
