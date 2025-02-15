/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_arguments.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:45/48 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 15:45:48 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	replace_list_next_with_new_arg(t_list *list, t_list *new)
{
	t_list	*tmp;

	ft_lstlast(new)->next = list->next->next;
	tmp = list->next;
	list->next = new;
	tmp->next = NULL;
	ft_lstclear(&tmp, free_arguments);
}

t_list	*handle_wildcard_argument(t_argument argument)
{
	char	*dir_path;
	char	*pattern;
	t_list	*dir_entries;
	t_list	*new_arguments;

	dir_path = get_dir_path(argument.word);
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
			replace_list_next_with_new_arg(head, new);
		}
		head = head->next;
	}
	*list = dummy.next;
	return (EXIT_SUCCESS);
}

void	expand_env_in_args(t_list *arguments, t_data *data)
{
	t_list		*head;
	char		*new_word;
	t_argument	*a;

	head = arguments;
	while (head)
	{
		a = (t_argument *)head->content;
		new_word = expand_string(a->word, data);
		free(a->word);
		a->word = new_word;
		head = head->next;
	}
}

int	expand_arguments(t_cmd_node *cmd_node, t_data *data)
{
	if (!cmd_node->arguments)
		return (EXIT_SUCCESS);
	expand_env_in_args(cmd_node->arguments, data);
	if (expand_wildcards_in_arguments(&(cmd_node->arguments)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
