/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirections_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:13/36 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/29 18:13:36 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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
			return (EXIT_FAILURE);
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
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
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
		if (pipe(hered_pipe) == -1)
		{
			perror("pipe");
			close_fds(fds);
			return (EXIT_FAILURE);
		}
		write(hered_pipe[1], r.word, ft_strlen(r.word));
		close(hered_pipe[1]);
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = hered_pipe[0];
	}
	return (EXIT_SUCCESS);
}

void	replace_list_next_with_new_redirection(t_list *list, t_list *new)
{
	t_list	*tmp;

	ft_lstlast(new)->next = list->next->next;
	tmp = list->next;
	list->next = new;
	tmp->next = NULL;
	ft_lstclear(&tmp, free_redirection);
}

t_list	*ignore_wildcard_in_redirection(t_redirection redirection)
{
	t_redirection	*new;

	new = (t_redirection *)malloc(sizeof(t_redirection));
	new->word = ft_strdup(redirection.word);
	new->word_type = LITERAL;
	new->type = redirection.type;
	new->fd = redirection.fd;
	return (ft_lstnew(new));
}

t_list	*filter_entries(t_list *dir_entries, char *pattern)
{
	char	*entry;
	t_list	dummy;
	t_list	*prev;
	t_list	*curr;

	dummy = (t_list){NULL, dir_entries};
	prev = &dummy;
	curr = dummy.next;
	while (curr)
	{
		entry = (char *)curr->content;
		if (!matches_pattern(pattern, entry) || !hidden_n_star(entry, pattern))
		{
			prev->next = curr->next;
			curr->next = NULL;
			ft_lstclear(&curr, free);
			curr = prev->next;
		}
		else
		{
			prev = prev->next;
			curr = curr->next;
		}
	}
	return (dummy.next);
}
