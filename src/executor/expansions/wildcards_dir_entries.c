/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:43/39 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 19:43:39 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_expansions.h"
#include <dirent.h>

char	*get_dir_path(char *path)
{
	char	*dir_path;
	char	*slash;

	slash = ft_strrchr(path, '/');
	if (slash)
		dir_path = ft_substr(path, 0, slash - path);
	else
		dir_path = ft_strdup("");
	return (dir_path);
}

t_list	*get_dir_entries(char *dir_path)
{
	DIR				*dir;
	t_list			*entries;
	struct dirent	*entry;

	if (!ft_strcmp(dir_path, ""))
		dir = opendir(".");
	else
		dir = opendir(dir_path);
	entries = NULL;
	entry = readdir(dir);
	while (entry != NULL)
	{
		ft_lstadd_back(&entries, ft_lstnew(ft_strdup(entry->d_name)));
		entry = readdir(dir);
	}
	closedir(dir);
	return (entries);
}

bool	hidden_n_star(char *entry, char *pattern)
{
	if (entry[0] == '.' && pattern[0] == '*')
		return (false);
	return (true);
}

t_list	*parse_dir_entries(t_list *dir_entries, char *dir_path, char *pattern)
{
	t_list		*new_arguments;
	char		*entry;
	t_argument	*new;

	new_arguments = NULL;
	while (dir_entries)
	{
		entry = (char *)dir_entries->content;
		if (matches_pattern(pattern, entry) && hidden_n_star(entry, pattern))
		{
			new = get_new_argument(entry, dir_path);
			if (!new)
			{
				ft_lstclear(&new_arguments, free_arguments);
				return (NULL);
			}
			ft_lstadd_back(&new_arguments, ft_lstnew(new));
		}
		dir_entries = dir_entries->next;
	}
	return (new_arguments);
}

char	*build_full_path(const char *dir_path, const char *entry_name)
{
	char		*full_path;
	char		*ptr;
	const char	*src = dir_path;

	if (!ft_strcmp(dir_path, ""))
		return (ft_strdup(entry_name));
	full_path = malloc(ft_strlen(dir_path) + ft_strlen(entry_name) + 2);
	if (!full_path)
		return (NULL);
	ptr = full_path;
	while (*src)
		*ptr++ = *src++;
	*ptr++ = '/';
	src = entry_name;
	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return (full_path);
}
