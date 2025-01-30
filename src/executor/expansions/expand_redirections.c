/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:40/12 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/17 12:40:12 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "executor_expansions.h"
#include "lexer.h"
#include "libft.h"
#include "parser.h"
#include <fcntl.h>
#include <unistd.h>

t_list	*handle_wildcard_redirection(t_redirection redirection)
{
	char	*dir_path;
	char	*pattern;
	t_list	*dir_entries;
	t_list	*new;

	dir_path = get_dir_path(redirection.word);
	pattern = get_pattern(redirection.word);
	if (!is_current_dir(dir_path))
		new = ignore_wildcard_in_redirection(redirection);
	else
	{
		dir_entries = get_dir_entries(dir_path);
		dir_entries = filter_entries(dir_entries, pattern);
		if (ft_lstsize(dir_entries) > 1)
			return (teardown_redirect(dir_path, pattern, dir_entries));
		if (ft_lstsize(dir_entries) == 0)
			new = ignore_wildcard_in_redirection(redirection);
		else
			new = new_redirection_from_entry((char *)dir_entries->content,
					redirection);
	}
	ft_lstclear(&dir_entries, free);
	free(dir_path);
	free(pattern);
	return (new);
}

int	expand_wildcards_in_redirections(t_list **list)
{
	t_list			*head;
	t_list			*new;
	t_list			dummy;
	t_redirection	*next;

	if (!list || !*list)
		return (EXIT_FAILURE);
	dummy = (t_list){NULL, *list};
	head = &dummy;
	while (head->next)
	{
		next = (t_redirection *)head->next->content;
		if (next->word_type == WILDCARD_EXP)
		{
			new = handle_wildcard_redirection(*next);
			if (!new)
				return (EXIT_FAILURE);
			replace_list_next_with_new_redirection(head, new);
		}
		head = head->next;
	}
	*list = dummy.next;
	return (EXIT_SUCCESS);
}

void	expand_env_redirection(t_list *redirections, t_data *data)
{
	t_list			*head;
	char			*new_word;
	t_redirection	*r;

	head = redirections;
	while (head)
	{
		r = (t_redirection *)head->content;
		if (r->word_type == SINGLE_QUOTE)
			new_word = ft_strdup(r->word);
		else if (r->word_type == DOUBLE_QUOTE)
			new_word = handle_double_quotes_hd(r->word, data);
		else
			new_word = expand_string(r->word, data);
		free(r->word);
		r->word = new_word;
		head = head->next;
	}
}

int	expand_redirections(t_cmd_node *cmd_node, t_data *data)
{
	if (!cmd_node->redirections)
		return (EXIT_SUCCESS);
	expand_env_redirection(cmd_node->redirections, data);
	if (expand_wildcards_in_redirections(&cmd_node->redirections))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	set_redirections(t_list **redirections, int fds[2])
{
	t_list			*head;
	t_redirection	r;
	int				hered_pipe[2];

	head = *redirections;
	while (head)
	{
		r = *((t_redirection *)head->content);
		if (set_input(r, fds, hered_pipe) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (set_output(r, fds, hered_pipe) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		head = head->next;
	}
	return (0);
}
