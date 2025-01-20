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
#include "parser.h"
#include <fcntl.h>
#include <unistd.h>

void	close_fds(int fds[2])
{
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	return ;
}

int	set_output(t_redirection r, int fds[2], int hered_pipe[2])
{
	(void)hered_pipe;
	if (r.type == OUT)
	{
		fds[1] = open(r.word, O_CREAT | O_WRONLY | O_TRUNC,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fds[1] < 0)
		{
			perror("open");
			close_fds(fds);
			return EXIT_FAILURE;
		}
	}
	if (r.type == APPEND)
	{
		fds[1] = open(r.word, O_APPEND | O_WRONLY | O_CREAT,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fds[1] < 0)
		{
			perror("open");
			close_fds(fds);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int	set_input(t_redirection r, int fds[2], int hered_pipe[2])
{
	if (r.type == IN)
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = open(r.word, O_RDONLY, NULL);
		if (fds[0] < 0)
		{
			perror("open");
			close_fds(fds);
			return (EXIT_FAILURE);
		}
	}
	if (r.type == HERED)
	{
		pipe(hered_pipe);
		write(hered_pipe[1], r.word, ft_strlen(r.word));
		close(hered_pipe[1]);
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = hered_pipe[0];
	}
	return (EXIT_SUCCESS);
}

t_list	*ignore_wildcard_in_redirection(char *dir_path, char *pattern)
{
	char		*full_path;
	t_argument	*new;

	full_path = build_full_path(dir_path, pattern);
	new = (t_argument *)malloc(sizeof(t_argument));
	new->word = full_path;
	new->type = LITERAL;
	return (ft_lstnew(new));
}
t_list	*handle_wildcard_redirection(t_redirection redirection)
{
	char	*dir_path;
	char	*pattern;
	t_list	*dir_entries;
	t_list	*new_arguments;

	dir_path = get_dir_path_2(redirection.word);
	pattern = get_pattern(redirection.word);
	if (ft_strcmp(dir_path, "") && ft_strcmp(dir_path, "."))
		new_arguments = ignore_wildcard(dir_path, pattern);
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

int	expand_wildcards_in_redirections(t_list **list)
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
		if (((t_redirection *)head->next->content)->word_type == WILDCARD_EXP)
		{
			new = handle_wildcard_redirection(*(t_redirection *)head->next->content);
			if (!new)
				return (EXIT_FAILURE);
			replace_list_next_with_new(head, new);
		}
		head = head->next;
	}
	*list = dummy.next;
	return (EXIT_SUCCESS);
}


int	set_redirections(t_list *redirections, int fds[2])
{
	t_list			*head;
	t_redirection	r;
	int				hered_pipe[2];

	head = redirections;
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
