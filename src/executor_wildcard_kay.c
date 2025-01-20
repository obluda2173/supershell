/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_wildcard_kay.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 21:16/16 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/19 21:16:16 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>

char	*get_dir_path_2(char* path)
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

char	*get_pattern(char* word)
{
	char	*pattern;
	char	*slash;

	slash = ft_strrchr(word, '/');
	if (slash)
		pattern = strdup(slash + 1);
	else
		pattern = ft_strdup(word);
	return (pattern);
}

t_list	*get_dir_entries(char *dir_path)
{
	DIR				*dir;
	t_list			*entries;
	struct dirent	*entry;

	if (!ft_strcmp(dir_path, ""))
	{
		dir = opendir(".");
	}
	else
	{
		dir = opendir(dir_path);
	}
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

bool	no_hidden_file(char *entry, char *pattern)
{
	if (!ft_strcmp(entry, ".") || !ft_strcmp(entry, "..") || (entry[0] == '.'
			&& pattern[0] == '*'))
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
		if (matches_pattern(pattern, entry) && no_hidden_file(entry, pattern))
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

t_list	*create_wildcard_arguments(t_list *dir_entries, char *dir_path,
		char *pattern)
{
	t_list	*new_arguments;

	new_arguments = parse_dir_entries(dir_entries, dir_path, pattern);
	if (!new_arguments)
		return (ignore_wildcard_in_argument(dir_path, pattern));
	return (new_arguments);
}

t_list	*handle_wildcard_argument(t_argument argument)
{
	char	*dir_path;
	char	*pattern;
	t_list	*dir_entries;
	t_list	*new_arguments;

	dir_path = get_dir_path_2(argument.word);
	pattern = get_pattern(argument.word);
	if (ft_strcmp(dir_path, "") && ft_strcmp(dir_path, "."))
		new_arguments = ignore_wildcard_in_argument(dir_path, pattern);
	else
	{
		dir_entries = get_dir_entries(dir_path);
		new_arguments = create_wildcard_arguments(dir_entries, dir_path,
				pattern);
		ft_lstclear(&dir_entries, free);
	}
	free(dir_path);
	free(pattern);
	return (new_arguments);
}

void	replace_list_next_with_new(t_list *list, t_list *new)
{
	t_list	*tmp;

	ft_lstlast(new)->next = list->next->next;
	tmp = list->next;
	list->next = new;
	tmp->next = NULL;
	ft_lstclear(&tmp, free_arguments);
}

int	expand_wildcards_in_arguments(t_list **list)
{
	t_list	*head;
	t_list	*new;
	t_list	dummy;

	if (!list || !*list)
		return (EXIT_FAILURE);
	dummy = (t_list){NULL, *list};
	head = &dummy;
	while (head->next)
	{
		if (((t_argument *)head->next->content)->type == WILDCARD_EXP)
		{
			new = handle_wildcard_argument(*(t_argument *)head->next->content);
			if (!new)
				return (EXIT_FAILURE);
			sort_arguments(&new);
			replace_list_next_with_new(head, new);
		}
		head = head->next;
	}
	*list = dummy.next;
	return (EXIT_SUCCESS);
}
