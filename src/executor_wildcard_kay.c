#include "executor.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>

char	*get_dir_path_2(t_argument argument)
{
	char	*dir_path;
	char	*slash;

	slash = ft_strrchr(argument.word, '/');
	if (slash)
		dir_path = ft_substr(argument.word, 0, slash - argument.word);
	else
		dir_path = ft_strdup(".");
	return (dir_path);
}

char	*get_pattern(t_argument argument)
{
	char	*pattern;
	char	*slash;

	slash = ft_strrchr(argument.word, '/');
	if (slash)
		pattern = strdup(slash + 1);
	else
		pattern = ft_strdup(argument.word);
	return (pattern);
}

t_list	*get_dir_entries(char *dir_path)
{
	DIR				*dir;
	t_list			*entries;
	struct dirent	*entry;

	dir = opendir(dir_path);
	entries = NULL;
	while ((entry = readdir(dir)) != NULL)
		ft_lstadd_back(&entries, ft_lstnew(ft_strdup(entry->d_name)));
	closedir(dir);
	return (entries);
}

t_list	*handle_wildcard_argument(t_argument argument)
{
	char		*dir_path;
	char		*pattern;
	t_list		*dir_entries;
	t_list		*head;
	t_list		*new_arguments;
	char		*entry;
	char		*full_path;
	t_argument	*new;

	dir_path = get_dir_path_2(argument);
	pattern = get_pattern(argument);
	dir_entries = get_dir_entries(dir_path);
	head = dir_entries;
	new_arguments = NULL;
	while (head)
	{
		entry = (char *)head->content;
		if (matches_pattern(pattern, entry))
		{
			if (!ft_strcmp(entry, ".") || !ft_strcmp(entry, "..")
				|| (entry[0] == '.' && pattern[0] == '*'))
			{
				head = head->next;
				continue ;
			}
			full_path = build_full_path(dir_path, entry);
			new = (t_argument *)malloc(sizeof(t_argument));
			new->word = full_path;
			new->type = LITERAL;
			ft_lstadd_back(&new_arguments, ft_lstnew(new));
		}
		head = head->next;
	}
	free(dir_path);
	free(pattern);
	ft_lstclear(&dir_entries, free);
	return (new_arguments);
}


void	replace_next_with_new(t_list *list, t_list *new)
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
			replace_next_with_new(head, new);
		}
		head = head->next;
	}
	*list = dummy.next;
	return (EXIT_SUCCESS);
}
