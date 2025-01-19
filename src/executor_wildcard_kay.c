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
	return dir_path;
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
	return pattern;
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

t_list *handle_wildcard_argument(t_argument argument) {
	char* dir_path = get_dir_path_2(argument);
	char* pattern = get_pattern(argument);
	t_list *dir_entries = get_dir_entries(dir_path);
	t_list *head = dir_entries;
	t_list *new_arguments = NULL;
	while (head) {
		char* entry = (char*)head->content;
		if (matches_pattern(pattern, entry)) {
			char *full_path = build_full_path(dir_path , entry);
			t_argument *new = (t_argument*)malloc(sizeof(t_argument));
			new->word = full_path;
			new->type = LITERAL;
			ft_lstadd_back(&new_arguments, ft_lstnew(new));
		}
		head = head->next;
	}

	free(dir_path);
	free(pattern);
	ft_lstclear(&dir_entries, free);
	return new_arguments;
}


void sort_wildcard(t_list **list) {
	t_list *head;
	if (!list || !*list)
		return;

	int len = ft_lstsize(*list);

	t_list dummy = {NULL, *list};
	head = &dummy;
	while (len > 1) {
		t_list *prev = head;
		t_list *curr = prev->next;

		int count = 0;
		while (count < len-1) {
			count++;
			if (ft_strcmp(((t_argument*)curr->content)->word, ((t_argument*)curr->next->content)->word) < 0) {
				prev = curr;
				curr = curr->next;
			} else {
				prev->next = curr->next;
				curr->next = curr->next->next;
				prev->next->next = curr;
				curr = prev->next->next;
				prev = prev->next;
			}
		}
		len--;

	}

	*list = dummy.next;
}

int expand_wildcards_in_arguments(t_list **list) {
	t_list *head = *list;
	if (!head)
		return EXIT_FAILURE;

	if (((t_argument*)head->content)->type == WILDCARD_EXP) {
		t_list* new = handle_wildcard_argument(*(t_argument*)head->content);
		if (!new)
			return EXIT_FAILURE;
		sort_wildcard(&new);
		ft_lstlast(new)->next = head->next;
		*list = new;
		head->next = NULL;
		ft_lstclear(&head, free_arguments);
		head = *list;
	}

	while (head->next) {
		if (((t_argument*)head->next->content)->type == WILDCARD_EXP) {
			t_list* new = handle_wildcard_argument(*(t_argument*)head->next->content);
			if (!new)
				return EXIT_FAILURE;
			ft_lstlast(new)->next = head->next->next;
			t_list *tmp = head->next;
			head->next = new;
			tmp->next = NULL;
			ft_lstclear(&tmp, free_arguments);
		}
		head = head->next;
	}
	return EXIT_SUCCESS;
}

